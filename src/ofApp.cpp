#include "ofApp.h"
#include "ofxAssimpModelLoader.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(ofColor::darkGrey);
	cam.setDistance(10);
	cam.setNearClip(.1);
	ofEnableDepthTest();

	boidModel.parseFile("geo/bird-02_blend.obj", vertices, faces);
	std::cout<< faces.size() << std::endl;
	for (int i = 0; i < faces.size(); i++) {
		/*std::cout << vertices[i] << std::endl;*/
		/*std::cout << "Face: " << i << " " << faces[i] << std::endl;*/
		
	}

	// Create multiple boxes and add them to the vector
	voxels.push_back(Voxel(glm::vec3(0, 0, 0), 1)); // Center
	voxels.push_back(Voxel(glm::vec3 (- 2, 0, 0), 1)); // Left
	voxels.push_back(Voxel(glm::vec3(2, 0, 0), 1)); // Right
	voxels.push_back(Voxel(glm::vec3(0, -2, 0), 1)); // Top
	voxels.push_back(Voxel(glm::vec3(0, 2, 0), 1)); // Bottom


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


}

//--------------------------------------------------------------
void ofApp::update(){

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
