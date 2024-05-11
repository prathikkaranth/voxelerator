#pragma once

#include<thread>
#include<chrono>
#include<atomic>

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#include "ofMesh.h"
#include "voxel.h"
#include "ofxGui.h"
#include "ofxDropdown.h"
#include "ray.h"
#include "aabb.h"
#include "bvh.h"
#include "hittable.h"
#include "triangle.h"

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
		void voxelerateMesh(const std::shared_ptr<hittable>& hitBVH, aabb bbox);
		void parallelVoxelRay(const std::shared_ptr<hittable>& bvh, aabb bbox, int start, int end);
		std::shared_ptr<hittable> scene();

		ofEasyCam cam;

		// GUI
		ofxPanel gui;
		ofParameter<ofVec3f> uiPosition;
		ofParameter<float> roundBoxSize;
		ofParameter<float> legoBlockSize;
		ofParameter<float> sphereSize;

		ofxDropdown_<string> boxModelType{ "boxModelType" };

		// Lights
		//
		ofLight light1;
		bool modelLoaded = false;

		bool drawModel = true;
		bool voxelerate = false;

		bool roundBox = false;
		bool legoBlock = false;
		bool ofBox = false;

		std::vector<Voxel> voxels;

		ofxAssimpModelLoader model;
		ofMaterial modelMaterial;

		ofxAssimpModelLoader roundBoxModel;

		ofxAssimpModelLoader legoBlockModel;

		ofxAssimpModelLoader sphereModel;

		ofBoxPrimitive boundingBox;


};
