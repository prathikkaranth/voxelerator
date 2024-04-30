#pragma once

#include "ofMain.h"
#include "vector3.h"
#include "ray.h"

#include <glm/gtx/intersect.hpp>

class Voxel {
	public:
		Voxel(glm::vec3 pos, float size);
		void draw();
		void voxelRay(const ofMesh& mesh);
		bool rayTriangleIntersection(Ray ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
		bool intersectsMesh(const ofMesh& mesh);

		glm::vec3 mPosition;

	private:
		ofBoxPrimitive voxel;
};