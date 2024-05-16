#include "ofApp.h"
#include "ofxAssimpModelLoader.h"


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(ofColor::lightSkyBlue);
	cam.setNearClip(.1);
	ofEnableDepthTest();

	//Cam
	//
	cam.setPosition(0, 0, -10);
	/*cam.setPosition(-9, 3, 0);*/
	cam.lookAt(glm::vec3(0, 3, 0));

	// GUI
	//
	gui.setup();
	gui.add(uiPosition.set("Light Position", ofVec3f(0, 3.67347, -5.20408), ofVec3f(-30, -30, -30), ofVec3f(30, 30, 30)));
	gui.add(roundBoxSize.set("RoundBox Size", 0.0003205, 0.0001, 0.001));
	gui.add(legoBlockSize.set("LegoBlock Size", 0.000361, 0.0001, 0.001));
	gui.add(sphereSize.set("Sphere Size", 0.0003205, 0.0001, 0.001));
	gui.add(boxSize.set("Box Size", 0.0003205, 0.0001, 0.001));

	gui.add(&boxModelType);
	boxModelType.add("RoundBox");
	boxModelType.add("LegoBlock");
	boxModelType.add("Box");
	boxModelType.add("Sphere");

	boxModelType.setSelectedValueByName("RoundBox", 0);

	// Main model
	if (!model.loadModel("geo/Bonsai/model.obj")) {
		cout << "model not found" << endl;
		ofExit();
	}

	else {
		modelLoaded = true;
	}

	// VoxelBox
	//
	if (!roundBoxModel.loadModel("geo/RoundCube/RoundCube.obj")) {
		cout << "model not found" << endl;
		ofExit();
	}

	if (!legoBlockModel.loadModel("geo/LEGO/model.obj")) {
		cout << "model not found" << endl;
		ofExit();
	}

	if (!sphereModel.loadModel("geo/Sphere/sphere.obj")) {
		cout << "model not found" << endl;
		ofExit();
	}

	if (!boxModel.loadModel("geo/Cube/Cube.obj")) {
		cout << "model not found" << endl;
		ofExit();
	}

	// First time setup
	model.setRotation(0, 180, 0, 0, 1);
	model.setScale(0.03, 0.03, 0.03);

	roundBoxModel.setRotation(0, 180, 1, 0, 0);
	roundBoxModel.setScale(roundBoxSize, roundBoxSize, roundBoxSize);

	legoBlockModel.setRotation(0, 180, 1, 0, 0);
	legoBlockModel.setScale(legoBlockSize, legoBlockSize+0.0001, legoBlockSize);

	sphereModel.setRotation(0, 180, 1, 0, 0);
	sphereModel.setScale(sphereSize, sphereSize, sphereSize);

	boxModel.setRotation(0, 180, 1, 0, 0);
	boxModel.setScale(boxSize, boxSize, boxSize);

	
	// Set up lighting
	//
	ofSetSmoothLighting(true);

	// setup one point light
	//
	light1.enable();
	/*light1.setPosition(0, 10, 0);*/
	light1.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
	light1.setSpecularColor(ofColor(125.f, 125.f, 125.f));
	light1.setAmbientColor(ofColor(150.0f, 150.0f, 150.0f));

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

	const int gridSize = 100;  // grid resolution

	const float bboxWidth = bbox.max().x - bbox.min().x;
	const float bboxHeight = bbox.max().y - bbox.min().y;
	const float bboxDepth = bbox.max().z - bbox.min().z;

	int gridSizeX, gridSizeY, gridSizeZ;
	if (bboxWidth > bboxHeight && bboxWidth > bboxDepth) {
		gridSizeX = gridSize;
		gridSizeY = gridSize * bboxHeight / bboxWidth;
		gridSizeZ = gridSize * bboxDepth / bboxWidth;
	}
	else if (bboxHeight > bboxWidth && bboxHeight > bboxDepth) {
		gridSizeX = gridSize * bboxWidth / bboxHeight;
		gridSizeY = gridSize;
		gridSizeZ = gridSize * bboxDepth / bboxHeight;
	}
	else {
		gridSizeX = gridSize * bboxWidth / bboxDepth;
		gridSizeY = gridSize * bboxHeight / bboxDepth;
		gridSizeZ = gridSize;
	}

	const float voxelSizeX = bboxWidth / gridSizeX;
	const float voxelSizeY = bboxHeight / gridSizeY;
	const float voxelSizeZ = bboxDepth / gridSizeZ;

	const float bboxMin = std::min(std::min(bboxWidth, bboxHeight), bboxDepth);
	const float boxPrimSize = bboxMin * 0.1 / 3.18903;

	for (int x = 0; x < gridSizeX; x++) {
		for (int y = 0; y < gridSizeY; y++) {
			for (int z = 0; z < gridSizeZ; z++) {

				// gridx == 0 -> worldX = bbox.min().x, gridx == 1 --> worldX = bbox.min().x + voxelSizeX
				const float worldX = bbox.min().x + x * voxelSizeX;
				const float worldY = bbox.min().y + y * voxelSizeY;
				const float worldZ = bbox.min().z + z * voxelSizeZ;

				float offset = 0.50;
				glm::vec3 position(worldX, worldY, worldZ);
				/*glm::vec3 position(worldX + offset * voxelSizeX, worldY + offset * voxelSizeY, worldZ + offset * voxelSizeZ);*/
				voxels.push_back(Voxel(position, boxPrimSize));
			}
		}
	}

	const auto numThreads = 32;
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
	roundBoxModel.setScale(roundBoxSize, roundBoxSize, roundBoxSize);
	legoBlockModel.setScale(legoBlockSize, legoBlockSize + 0.0001, legoBlockSize);
	sphereModel.setScale(sphereSize, sphereSize, sphereSize);
	boxModel.setScale(boxSize, boxSize, boxSize);


	if (boxModelType.selectedValue.get() == "Box")
		light1.setAmbientColor(ofColor(10.0f, 10.0f, 10.0f));
	else if(drawModel)
		light1.setAmbientColor(ofColor(10.0f, 10.0f, 10.0f));
	else
		light1.setAmbientColor(ofColor(150.0f, 150.0f, 150.0f));
}

//--------------------------------------------------------------
void ofApp::draw(){


	// draw everything in 3D through camera
	//
	cam.begin();
	
		ofEnableDepthTest();

		// draw grid
		if (gridVisible) {
			ofPushMatrix();
			ofSetColor(ofColor::dimGray);
			ofRotateDeg(90);
			ofDrawGridPlane();
			ofPopMatrix();
		}

		// draw light
		ofPushMatrix();
		ofDrawSphere(light1.getPosition(), 0.1);
		ofPopMatrix();

		// draw bounding box
		if (boundingBoxVisible) {
			ofPushMatrix();
			ofSetColor(ofColor::white);
			ofNoFill();
			boundingBox.drawWireframe();
			ofPopMatrix();
		}

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

				if (boxModelType.selectedValue.get() == "RoundBox") {	
					voxel.draw(roundBoxModel);
				}

				else if (boxModelType.selectedValue.get() == "LegoBlock")
					voxel.draw(legoBlockModel);

				else if (boxModelType.selectedValue.get() == "Box")
					voxel.draw(boxModel);

				else if (boxModelType.selectedValue.get() == "Sphere")
					voxel.draw(sphereModel);

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
		drawModel = false;
		voxelerate = true;
	}

	if (key == 'm') {
		drawModel = true;
		voxelerate = false;
	}

	if (key == 'b') {
		boundingBoxVisible = !boundingBoxVisible;
	}

	if (key == 'g') {
		gridVisible = !gridVisible;
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
