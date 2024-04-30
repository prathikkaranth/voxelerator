#include "ofApp.h"
#include "ofxAssimpModelLoader.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(ofColor::darkGrey);
	cam.setDistance(10);
	cam.setNearClip(.1);
	ofEnableDepthTest();

	//boidModel.parseFile("geo/bird-02_blend.obj", vertices, faces);

	//for (int i = 0; i < vertices.size(); i++) {
	//	/*std::cout << vertices[i] << std::endl;*/
	//	/*voxels.push_back(Voxel(vertices[i], 0.25));*/
	//}

	//for (int i = 0; i < faces.size(); i++) {
	//	/*std::cout << "Face: " << i << " " << faces[i] << std::endl;*/
	//}


	if (!model.loadModel("geo/suzzane.obj")) {
		cout << "Can't load model" << endl;
		ofExit();
	}


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
	voxels.push_back(Voxel(glm::vec3(0, 4, 0), 0.25));
}

//--------------------------------------------------------------
void ofApp::update(){

	for (int i = 0; i < voxels.size(); i++) {
		voxels[i].voxelRay(model.getMesh(0));
		voxels[i].mPosition.x = 5 * sin(ofGetElapsedTimef());
	}

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
		model.setScale(0.01, 0.01, 0.01);
		birdMaterial.begin();
		ofSetColor(ofColor::red);
		model.enableMaterials();
		model.enableColors();
		model.enableNormals();
		model.drawFaces();
		birdMaterial.end();
		ofDisableLighting();
		ofPopMatrix();

		// draw voxels
		
		for (auto& voxel : voxels) {
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
