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
		voxelizeMesh();
	}

}

void ofApp::voxelizeMesh() {
	int gridSize = 30;
	float spacing = 0.1;
	float voxelSize = 0.10;
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
		ofPushMatrix();
		ofEnableLighting();
		birdMaterial.setDiffuseColor(ofColor::red);
		
		birdMaterial.begin();
		ofSetColor(ofColor::red);
		model.enableMaterials();
		model.enableColors();
		model.enableNormals();
		/*model.drawFaces();*/
		ofSetColor(ofColor::blue);
		model.drawWireframe();
		birdMaterial.end();
		ofDisableLighting();
		ofPopMatrix();

		// draw voxels
		
		for (auto& voxel : voxels) {
			/*voxel.draw();*/
			voxel.draw();
		}
		

	cam.end();

	// draw UI
	//

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	
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
