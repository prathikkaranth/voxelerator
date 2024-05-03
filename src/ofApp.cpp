#include "ofApp.h"
#include "ofxAssimpModelLoader.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(ofColor::darkGrey);
	cam.setDistance(10);
	cam.setNearClip(.1);
	ofEnableDepthTest();

	if (!model.loadModel("geo/suzzane.obj")) {
		cout << "Can't load model" << endl;
		ofExit();
	}

	else {
		modelLoaded = true;
	}

	model.setScale(0.01, 0.01, 0.01);
	/*model.setRotation(0, 180, 1, 0, 0);*/
	// Set up lighting
	//
	ofSetSmoothLighting(true);

	// setup one point light
	//
	light1.enable();
	light1.setPosition(0, 20, 0);
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
	int gridSize = 30;
	float spacing = 0.185;
	float voxelSize = 0.15;

	// Cherry Values
	/*int gridSize = 50;
	float spacing = 0.045;
	float voxelSize = 0.045;*/

	for (int x = -gridSize; x <= gridSize; x++) {
		for (int y = -gridSize; y <= gridSize; y++) {
			for (int z = -gridSize; z <= gridSize; z++) {
				glm::vec3 position(x * spacing, y * spacing, z * spacing);
				voxels.push_back(Voxel(position, voxelSize));
			}
		}
	}
	for (int i = 0; i < voxels.size(); i++) {
		voxels[i].voxelRay(model.getMesh(0), model.getModelMatrix());
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	//for (int i = 0; i < voxels.size(); i++) {
	//	voxels[i].voxelRay(model.getMesh(0), model.getModelMatrix());
	//	/*voxels[i].mPosition.y = 5 * sin(ofGetElapsedTimef());*/
	//}
}

//--------------------------------------------------------------
void ofApp::draw(){


	// draw everything in 3D through camera
	//
	cam.begin();
	
		// draw grid
		ofPushMatrix();
		ofSetColor(ofColor::dimGray);
		ofRotateDeg(90);
		ofDrawGridPlane();
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

	cam.end();

	// draw UI
	//

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
