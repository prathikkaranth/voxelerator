#pragma once

#include "ofMain.h"
#include "vector3.h"
#include "ray.h"
#include "hittable.h"
#include "ofxAssimpModelLoader.h"

#include <glm/gtx/intersect.hpp>

class Voxel {
	public:
		Voxel(glm::vec3 pos, float size);
		void draw();
		void draw(ofxAssimpModelLoader &boxModel);
		void voxelRay(const glm::mat4& modelMatrix, const std::shared_ptr<hittable>& hitBVH);
		int intersectsMesh(const glm::mat4& modelMatrix, float &distOut, Ray ray, const std::shared_ptr<hittable>& hitBVH, ofFloatColor& colorOut);
		float shortestDistanceMesh(const glm::mat4& modelMatrix, const std::shared_ptr<hittable>& hitBVH);

		glm::vec3 mPosition;

		bool isVisible = false;
		ofFloatColor mColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	private:
		static ofBoxPrimitive sBoxPrimitive;
};