#include "fileParser.h"
#include "ofxAssimpMeshHelper.h"

fileParser::fileParser() {
}

fileParser::~fileParser() {
}

void fileParser::parseFile(std::string fileName, std::vector<glm::vec3>& storedVerts, std::vector<glm::uvec3>& storedFaces) {
	file.open(fileName);
	if (!file.exists()) {
		ofLogError("fileParser") << "File not found: " << fileName;
		std::cout << "file doesn't exists" << std::endl;
	}

	else {
		ofLogNotice("fileParser") << "Parsing file: " << fileName;
		std::cout<<"file exists"<<std::endl;

		ofxAssimpModelLoader modelLoader;
		modelLoader.loadModel(fileName);

		model = modelLoader.getMesh(0);
		for (int i = 0; i < model.getNumVertices(); i++) {				
			storedVerts.push_back(model.getVertex(i));
		}

		for (int i = 0; i < model.getNumIndices(); i+=3) {				
			glm::uvec3 face = glm::uvec3(model.getIndex(i), model.getIndex(i + 1), model.getIndex(i + 2));
			storedFaces.push_back(face);				
		}
		
	}
}
