#pragma once

#include "ofMain.h"

class Voxel {
	public:
		Voxel(glm::vec3 pos, float size);
		void draw();

	private:
		ofBoxPrimitive voxel;
};