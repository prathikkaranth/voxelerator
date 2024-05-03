#pragma once

#include "ofMain.h"
#include "vector3.h"
#include "ray.h"

#include <glm/gtx/intersect.hpp>

class Voxel {
	public:
		Voxel(glm::vec3 pos, float size);
		void draw();
		void voxelRay(const ofMesh& mesh, const glm::mat4& modelMatrix);
		bool rayTriangleIntersection(Ray ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
		int intersectsMesh(const ofMesh& mesh, const glm::mat4& modelMatrix);

		glm::vec3 mPosition;

		bool isVisible = false;

	private:
		static ofBoxPrimitive sBoxPrimitive;
};