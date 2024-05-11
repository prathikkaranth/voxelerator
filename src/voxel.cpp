#include "voxel.h"
#include <glm/gtx/intersect.hpp>

ofBoxPrimitive Voxel::sBoxPrimitive;

Voxel::Voxel(glm::vec3 pos, float size) {
	mPosition = pos;

	/*sBoxPrimitive.setPosition(pos);*/
	sBoxPrimitive.set(size);
}

void Voxel::draw() {
	/*sBoxPrimitive.setPosition(mPosition);*/

	if (isVisible) {
		sBoxPrimitive.setPosition(mPosition);
		sBoxPrimitive.setSideColor(0, mColor);
		sBoxPrimitive.setSideColor(1, mColor);
		sBoxPrimitive.setSideColor(2, mColor);
		sBoxPrimitive.setSideColor(3, mColor);
		sBoxPrimitive.setSideColor(4, mColor);
		sBoxPrimitive.setSideColor(5, mColor);
		sBoxPrimitive.draw();
	}
	static constexpr int voxelSize = sizeof(Voxel);
}

void Voxel::draw(ofxAssimpModelLoader &boxModel) {

	
	if (isVisible) {

		boxModel.setPosition(mPosition.x, mPosition.y, mPosition.z);

		ofxAssimpMeshHelper& meshHelper =  boxModel.getMeshHelper(0);
		meshHelper.material.setDiffuseColor(mColor);

		boxModel.drawFaces();
	}
	static constexpr int voxelSize = sizeof(Voxel);
}

void Voxel::voxelRay(const glm::mat4& modelMatrix, const std::shared_ptr<hittable>& hitBVH) {

	float shortestDist = shortestDistanceMesh(modelMatrix, hitBVH);

	if (shortestDist < 0.085) {
		isVisible = true;
	}
	else {
	}
	
	/*isVisible = true;*/
	
}

int Voxel::intersectsMesh(const glm::mat4& modelMatrix, float &distOut, Ray ray, const std::shared_ptr<hittable>& hitBVH, ofFloatColor& colorOut) {

	glm::vec2 bary;
	int intersectionCount = 0;

	hit_record rec;
	bool hitBV = hitBVH->hit(ray, 0.0001, std::numeric_limits<float>::max(), rec);

	if (hitBV) {
		intersectionCount++;
	}

	distOut = rec.t;
	colorOut = rec.color;
	return intersectionCount;
}

float Voxel::shortestDistanceMesh(const glm::mat4& modelMatrix, const std::shared_ptr<hittable>& hitBVH) {
	
	static constexpr std::array<glm::vec3, 6> directions = {
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(-1, 0, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, 0, -1)
	};

	float shortestDist = std::numeric_limits<float>::max();

	for (const auto& direction : directions) {
		float dist;
		ofFloatColor color;
		int intersectionCount = intersectsMesh(modelMatrix, dist, Ray(mPosition, direction), hitBVH, color);
		if (intersectionCount % 2 != 0 && dist < shortestDist) {
			shortestDist = dist;
			mColor = color;
		}
	}
	return shortestDist;

}




