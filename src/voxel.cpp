#include "voxel.h"
#include <glm/gtx/intersect.hpp>

Voxel::Voxel(glm::vec3 pos, float size) {
	mPosition = pos;

	voxel.setPosition(pos);
	voxel.set(size);
}

void Voxel::draw() {
	voxel.setPosition(mPosition);

	if (isVisible) {
		voxel.draw();
	}

	/*voxel.draw();*/
	
	/*ofSetColor(255, 255, 0);
	ofSetLineWidth(3);
	ofDrawLine(glm::vec3(mPosition.x, mPosition.y, mPosition.z), glm::vec3(mPosition.x, mPosition.y - 25, mPosition.z));*/

}

void Voxel::voxelRay(const ofMesh& mesh, const glm::mat4& modelMatrix) {

	// check how many times the ray intersects the mesh
	//
	//if (intersectsMesh(mesh, modelMatrix)) {
	//	/*std::cout << "true" << std::endl;*/
	//	voxel.setSideColor(3, ofColor(0, 255, 0));
	//	

	//}
	//else {
	//	std::cout << "false" << std::endl;
	//	ofSetColor(255, 0, 0);
	//	voxel.setSideColor(3, ofColor(255, 0, 0));
	//}

	int intersectCount = intersectsMesh(mesh, modelMatrix);
	
	if (intersectCount % 2 == 0) {
		/*std::cout << "outside mesh" << std::endl;*/
		voxel.setSideColor(3, ofColor(255, 0, 0));
		isVisible = false;

	}
	else {
		/*std::cout << "inside mesh" << std::endl;*/
		voxel.setSideColor(3, ofColor(0, 255, 0));
		isVisible = true;
	}
	
	
}

bool Voxel::rayTriangleIntersection(Ray ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {

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

	float t = invDet * glm::dot(edge2, q);

	if (t > 0.00001) {
		// intersection
		return true;
	}

	return false;
	
}

int Voxel::intersectsMesh(const ofMesh& mesh, const glm::mat4& modelMatrix) {

	glm::vec2 bary;
	float dist;
	int intersectionCount = 0;

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
		bool hit = rayTriangleIntersection(Ray(mPosition, glm::vec3(0, -1, 0)), v0, v1, v2);

		if (hit) {
			intersectionCount++;
		}
	}	

	return intersectionCount;
}



