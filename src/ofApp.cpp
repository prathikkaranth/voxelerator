#include "ofApp.h"
#include "ofxAssimpModelLoader.h"


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(ofColor::darkGrey);
	cam.setDistance(10);
	cam.setNearClip(.1);
	ofEnableDepthTest();

	//Cam
	//
	cam.setPosition(0, 0, -10);
	cam.lookAt(glm::vec3(0, 0, 0));

	// GUI
	//
	gui.setup();
	gui.add(uiPosition.set("Light Position", ofVec3f(0, 3.67347, -5.20408), ofVec3f(-30, -30, -30), ofVec3f(30, 30, 30)));

	gui.add(&boxModelType);
	boxModelType.add("RoundBox");
	boxModelType.add("LegoBlock");
	boxModelType.add("ofBox");

	boxModelType.setSelectedValueByName("RoundBox", 0);

	// Main model
	if (!model.loadModel("geo/Bonsai/model.obj")) {
		cout << "Can't load model" << endl;
		ofExit();
	}

	else {
		modelLoaded = true;
	}

	// VoxelBox
	//
	if (!roundBoxModel.loadModel("geo/RoundCube/RoundCube.obj")) {
		cout << "Can't load model" << endl;
		ofExit();
	}

	if (!legoBlockModel.loadModel("geo/LEGO/model.obj")) {
		cout << "Can't load model" << endl;
		ofExit();
	}

	model.setRotation(0, 180, 0, 0, 1);
	model.setScale(0.01, 0.01, 0.01);
	roundBoxModel.setRotation(0, 180, 1, 0, 0);
	roundBoxModel.setScale(0.0002, 0.0002, 0.0002);
	legoBlockModel.setRotation(0, 180, 1, 0, 0);
	legoBlockModel.setScale(0.0002, 0.0002, 0.0002);
	
	// Set up lighting
	//
	ofSetSmoothLighting(true);

	// setup one point light
	//
	light1.enable();
	/*light1.setPosition(0, 10, 0);*/
	light1.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
	light1.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	light1.setAmbientColor(ofColor(150, 150, 150));

	// spawn voxels in a grid 
	//
	if (modelLoaded) {
		const std::shared_ptr<hittable> bvh = scene();
		aabb bbox;
		bvh->bounding_box(0.0, 1.0, bbox);

		// draw bounding box
		//
		boundingBox.set(bbox.max().x - bbox.min().x, bbox.max().y - bbox.min().y, bbox.max().z - bbox.min().z);
		boundingBox.setPosition((bbox.max().x + bbox.min().x) / 2, (bbox.max().y + bbox.min().y) / 2, (bbox.max().z + bbox.min().z) / 2);
		
		voxelerateMesh(bvh, bbox);	
	}

}

void ofApp::parallelVoxelRay(const std::shared_ptr<hittable>& bvh, aabb bbox, int start, int end) {
	for (int i = start; i < end; ++i) {	
		voxels[i].voxelRay(model.getModelMatrix(), bvh);	
	}

}

std::shared_ptr<hittable> ofApp::scene() {
	hittable_list objects;
	for (int m = 0; m < model.getMeshCount(); m++) {
		ofMesh mesh = model.getMesh(m);
		ofMaterial material = model.getMaterialForMesh(m);
		ofFloatColor color = material.getDiffuseColor();
		for (int i = 0; i < mesh.getNumIndices(); i += 3) {
			glm::vec3 v0 = mesh.getVertex(mesh.getIndex(i));
			glm::vec3 v1 = mesh.getVertex(mesh.getIndex(i + 1));
			glm::vec3 v2 = mesh.getVertex(mesh.getIndex(i + 2));
			
			// transform vertices to world space
			v0 = glm::vec3(model.getModelMatrix() * glm::vec4(v0, 1));
			v1 = glm::vec3(model.getModelMatrix() * glm::vec4(v1, 1));
			v2 = glm::vec3(model.getModelMatrix() * glm::vec4(v2, 1));

			objects.add(
				std::make_shared<Triangle>(v0, v1, v2, color)
			);
		}
	}

	return make_shared<bvh_node>(objects, 0.0, 1.0);
}
	



void ofApp::voxelerateMesh(const std::shared_ptr<hittable>& bvh, aabb bbox) {

	const int gridSize = 37;

	const float voxelSizeX = (bbox.max().x - bbox.min().x) / gridSize;
	const float voxelSizeY = (bbox.max().y - bbox.min().y) / gridSize;
	const float voxelSizeZ = (bbox.max().z - bbox.min().z) / gridSize;
	const float boxPrimSize = 0.1;

	for (int x = 0; x < gridSize; x++) {
		for (int y = 0; y < gridSize; y++) {
			for (int z = 0; z < gridSize; z++) {

				// gridx == 0 -> worldX = bbox.min().x, gridx == 1 --> worldX = bbox.min().x + voxelSizeX
				const float worldX = bbox.min().x + x * voxelSizeX;
				const float worldY = bbox.min().y + y * voxelSizeY;
				const float worldZ = bbox.min().z + z * voxelSizeZ;

				float offset = 0.50;

				glm::vec3 position(worldX + offset * voxelSizeX, worldY + offset * voxelSizeY, worldZ + offset * voxelSizeZ);
				voxels.push_back(Voxel(position, boxPrimSize));
			}
		}
	}

	const auto numThreads = 16;
	const auto numVoxelsPerThread = voxels.size() / numThreads;

	std::vector<std::thread> threads;

	// Create threads
	for (int tid = 0; tid < numThreads; tid++) {
		const auto start = tid * numVoxelsPerThread;
		const auto end = tid == numThreads - 1 ? voxels.size() : start + numVoxelsPerThread;
		std::thread t(&ofApp::parallelVoxelRay, this, bvh, bbox, start, end);
		threads.push_back(std::move(t));
	}

	// Join threads
	for (auto& t : threads) {
		t.join();
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	light1.setPosition(uiPosition->x, uiPosition->y, uiPosition->z);
}

//--------------------------------------------------------------
void ofApp::draw(){


	// draw everything in 3D through camera
	//
	cam.begin();
	
		ofEnableDepthTest();
		// draw grid
		ofPushMatrix();
		ofSetColor(ofColor::dimGray);
		ofRotateDeg(90);
		ofDrawGridPlane();
		ofPopMatrix();

		// draw light
		ofPushMatrix();
		ofDrawSphere(light1.getPosition(), 0.1);
		ofPopMatrix();

		// draw bounding box
		ofPushMatrix();
		ofSetColor(ofColor::white);
		ofNoFill();
		boundingBox.drawWireframe();
		ofPopMatrix();

		// draw model
		if (drawModel) {
			ofPushMatrix();
			ofEnableLighting();
			modelMaterial.setDiffuseColor(ofColor::red);
			modelMaterial.begin();
			model.enableMaterials();
			model.enableColors();
			model.enableNormals();
			model.drawFaces();
			modelMaterial.end();
			ofDisableLighting();
			ofPopMatrix();
		}

		// draw voxels
		if (voxelerate) {
			ofPushMatrix();
			ofEnableLighting();
			for (auto& voxel : voxels) {
				
				/*roundBoxMaterial.begin();
				roundBoxModel.enableMaterials();
				roundBoxModel.enableColors();
				roundBoxModel.enableNormals();*/

				if (boxModelType.selectedValue.get() == "RoundBox") {
					//roundBoxMaterial.setDiffuseColor(ofColor(1.0, 0.0, 0.0));
					//roundBoxMaterial.begin();
					//roundBoxModel.enableMaterials();
					//roundBoxModel.enableColors();
					//roundBoxModel.enableNormals();
					voxel.draw(roundBoxModel);
					//roundBoxMaterial.end();
				}

				else if (boxModelType.selectedValue.get() == "LegoBlock")
					voxel.draw(legoBlockModel);

				else if (boxModelType.selectedValue.get() == "ofBox")
					voxel.draw();

			}
			ofDisableLighting();
			ofPopMatrix();
		}
		ofDisableDepthTest();

	cam.end();

	// draw UI
	//
	gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	if (key == 'v') {
		voxelerate = true;
		drawModel = false;
	}

	if (key == 'm') {
		drawModel = true;
		voxelerate = false;
	}
	
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
