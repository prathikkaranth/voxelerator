#include "ofApp.h"
#include "ofxAssimpModelLoader.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(ofColor::darkGrey);
	cam.setDistance(10);
	cam.setNearClip(.1);
	ofEnableDepthTest();

	// GUI
	//
	gui.setup();
	gui.add(uiPosition.set("Position", ofVec3f(0, 4, 0), ofVec3f(-30, -30, -30), ofVec3f(30, 30, 30)));

	if (!model.loadModel("geo/Bonsai/model.obj")) {
		cout << "Can't load model" << endl;
		ofExit();
	}

	else {
		modelLoaded = true;
	}

	model.setScale(0.01, 0.01, 0.01);
	model.setRotation(0, 5, 1, 0, 1);

	glm::vec3 minBound = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxBound = glm::vec3(std::numeric_limits<float>::min());;

	for (int m = 0; m < model.getMeshCount(); m++)
	{
		// Setup a bounding box for the model
		ofMesh mesh = model.getMesh(m);

		const glm::mat4 modelMatrix = model.getModelMatrix();
		for (int i = 0; i < mesh.getNumVertices(); i++) {

			glm::vec3 v = glm::vec3(modelMatrix * glm::vec4(mesh.getVertex(i), 1.0f));
			minBound.x = min(minBound.x, v.x);
			minBound.y = min(minBound.y, v.y);
			minBound.z = min(minBound.z, v.z);
			maxBound.x = max(maxBound.x, v.x);
			maxBound.y = max(maxBound.y, v.y);
			maxBound.z = max(maxBound.z, v.z);
		}
	}

	boundingBox.set(maxBound.x - minBound.x, maxBound.y - minBound.y, maxBound.z - minBound.z);
	boundingBox.setPosition((minBound + maxBound) / 2);
	
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

	/*voxels.push_back(Voxel(glm::vec3(6,7,0), 0.25));*/
	/*voxels.push_back(Voxel(glm::vec3(0, 0, 0), 0.25));*/
	// spawn voxels in a grid 
	//
	if (modelLoaded) {
		voxelerateMesh();
	}

}

void ofApp::voxelerateMesh() {

	// Suzanne Values
	/*int gridSize = 30;
	float spacing = 0.185;
	float voxelSize = 0.15;*/

	// Cherry Values
	/*int gridSize = 50;
	float spacing = 0.045;
	float voxelSize = 0.045;*/

	// Bonsai Values
	int gridSize = 30;
	float spacing = 0.15;
	float voxelSize = 0.15;


	for (int x = -gridSize; x <= gridSize; x++) {
		for (int y = -gridSize; y <= gridSize; y++) {
			for (int z = -gridSize; z <= gridSize; z++) {
				glm::vec3 position(x * spacing, y * spacing, z * spacing);
				voxels.push_back(Voxel(position, voxelSize));
			}
		}
	}
	for (int i = 0; i < voxels.size(); i++) {
		for (int m = 0; m < model.getMeshCount(); m++)
		{
			voxels[i].voxelRay(model.getMesh(m), model.getModelMatrix());
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	//for (int i = 0; i < voxels.size(); i++) {
	//	voxels[i].voxelRay(model.getMesh(0), model.getModelMatrix());
	//	/*voxels[i].mPosition.y = 5 * sin(ofGetElapsedTimef());*/
	//}

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


		// draw model
		if (drawModel) {
			ofPushMatrix();
			ofEnableLighting();
			birdMaterial.setDiffuseColor(ofColor::red);
			birdMaterial.begin();
			model.enableMaterials();
			model.enableColors();
			model.enableNormals();
			model.drawFaces();
			birdMaterial.end();
			ofDisableLighting();
			ofPopMatrix();
		}

		// draw bounding box
		ofPushMatrix();
		ofSetColor(ofColor::white);
		ofNoFill();
		boundingBox.drawWireframe();
		ofPopMatrix();

		// draw voxels
		ofPushMatrix();
		ofEnableLighting();
		if (voxelerate) {
			for (auto& voxel : voxels) {
				voxel.draw();
			}
		}
		ofDisableLighting();
		ofPopMatrix();

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
