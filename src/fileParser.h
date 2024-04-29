#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofMesh.h"


class fileParser{
	public:

		~fileParser();
		fileParser();

		void parseFile(std::string fileName, std::vector<glm::vec3>& storedVerts, std::vector<glm::uvec3>& storedFaces);

		ofFile file;
		ofMesh model;

};