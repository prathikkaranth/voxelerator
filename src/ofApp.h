#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "fileParser.h"
#include "ofMesh.h"
#include "voxel.h"

// special header file for all glm intersection functions
//
#include <glm/gtx/intersect.hpp>



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void voxelizeMesh();

		ofEasyCam cam;

		// Lights
		//
		ofLight light1;
		ofMaterial birdMaterial;
		bool modelLoaded = false;

		fileParser boidModel;

		std::vector<glm::vec3> vertices;
		std::vector<glm::uvec3> faces;

		std::vector<Voxel> voxels;

		ofxAssimpModelLoader model;
		
};
