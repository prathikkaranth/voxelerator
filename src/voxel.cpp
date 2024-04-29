#include "voxel.h"

Voxel::Voxel(glm::vec3 pos, float size) {
	voxel.setPosition(pos);
	voxel.set(size);
}

void Voxel::draw() {
	voxel.draw();
}