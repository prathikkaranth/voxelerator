#include "voxel.h"
#include <glm/gtx/intersect.hpp>

ofBoxPrimitive Voxel::sBoxPrimitive;

Voxel::Voxel(glm::vec3 pos, float size) {
	mPosition = pos;

	sBoxPrimitive.setPosition(pos);
	sBoxPrimitive.set(size);
}

void Voxel::draw() {
	sBoxPrimitive.setPosition(mPosition);

	if (isVisible) {
		sBoxPrimitive.draw();
	}
	static constexpr int voxelSize = sizeof(Voxel);
	
	/*voxel.draw();*/
	
	/*ofSetColor(255, 255, 0);
	ofSetLineWidth(3);
	ofDrawLine(glm::vec3(mPosition.x, mPosition.y, mPosition.z), glm::vec3(mPosition.x, mPosition.y - 25, mPosition.z));*/

}

void Voxel::voxelRay(const ofMesh& mesh, const glm::mat4& modelMatrix) {

	/*float dist;
	int intersectCount = intersectsMesh(mesh, modelMatrix, dist);
	
	if (intersectCount % 2 == 0) {
		isVisible = false;

	}
	else if(intersectCount % 2 != 0 && dist < 1.05) {
		isVisible = true;
	}*/
	float shortestDist = shortestDistanceMesh(Ray(mPosition, glm::vec3(0, -1, 0)), mesh, modelMatrix);

	if (shortestDist < 0.3) {
		isVisible = true;
	}
	else {
		isVisible = false;
	}
	
	
}

bool Voxel::rayTriangleIntersection(Ray ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float &t) {

	glm::vec3 edge1 = v1 - v0;
	glm::vec3 edge2 = v2 - v0;
	glm::vec3 h = glm::cross(ray.direction, edge2);
	float det = glm::dot(edge1, h);

	if (det > -0.00001 && det < 0.00001) return false;

	float invDet = 1.0 / det;
	glm::vec3 s = ray.origin - v0;
	float u = invDet * glm::dot(s, h);

	if (u < 0 || u > 1) return false;

	glm::vec3 q = glm::cross(s, edge1);
	float v = invDet * glm::dot(ray.direction, q);

	if (v < 0 || u + v > 1) return false;

	t = invDet * glm::dot(edge2, q);

	if (t > 0.00001) {
		return true;
	}
	
}

int Voxel::intersectsMesh(const ofMesh& mesh, const glm::mat4& modelMatrix, float &distOut, Ray ray) {

	glm::vec2 bary;
	int intersectionCount = 0;
	float shortestDist = std::numeric_limits <float>::max();

	for (unsigned int i= 0; i < mesh.getNumIndices(); i += 3) {
		glm::vec3 v0 = mesh.getVertex(mesh.getIndex(i));
		glm::vec3 v1 = mesh.getVertex(mesh.getIndex(i + 1));
		glm::vec3 v2 = mesh.getVertex(mesh.getIndex(i + 2));

		// transform vertices to world space
		v0 = glm::vec3(modelMatrix * glm::vec4(v0, 1));
		v1 = glm::vec3(modelMatrix * glm::vec4(v1, 1));
		v2 = glm::vec3(modelMatrix * glm::vec4(v2, 1));
		 

		/*bool hit = glm::intersectRayTriangle(mPosition, glm::vec3(0, -1, 0), 
			v0, v1, v2, bary, dist);*/
		float dist;
		/*bool hit = rayTriangleIntersection(Ray(mPosition, glm::vec3(0, -1, 0)), v0, v1, v2, dist);*/
		bool hit = rayTriangleIntersection(ray, v0, v1, v2, dist);

		if (hit) {
			intersectionCount++;
			shortestDist = std::min(dist, shortestDist);
		}
	}	

	distOut = shortestDist;
	return intersectionCount;
}

float Voxel::shortestDistanceMesh(Ray ray, const ofMesh& mesh, const glm::mat4& modelMatrix) {
	
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
		int intersectionCount = intersectsMesh(mesh, modelMatrix, dist, Ray(mPosition, direction));
		if (intersectionCount % 2 != 0) {
			shortestDist = std::min(dist, shortestDist);
		}
	}
	return shortestDist;

}



