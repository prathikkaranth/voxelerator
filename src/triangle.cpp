#include "triangle.h"

bool Triangle::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const{

	glm::vec3 edge1 = mVert1 - mVert0;
	glm::vec3 edge2 = mVert2 - mVert0;
	glm::vec3 h = glm::cross(r.direction, edge2);
	float det = glm::dot(edge1, h);

	if (det > -0.00001 && det < 0.00001) return false;

	float invDet = 1.0 / det;
	glm::vec3 s = r.origin - mVert0;
	float u = invDet * glm::dot(s, h);

	if (u < 0 || u > 1) return false;

	glm::vec3 q = glm::cross(s, edge1);
	float v = invDet * glm::dot(r.direction, q);

	if (v < 0 || u + v > 1) return false;

	float t = invDet * glm::dot(edge2, q);
	rec.t = t;

	if (t > 0.00001) {
		return true;
	}

	return false;
}

bool Triangle::bounding_box(double time0, double time1, aabb& output_box) const {
	
	double xmin = fmin(mVert0.x, mVert1.x);
	xmin = fmin(xmin, mVert2.x);

	double xmax = fmax(mVert0.x, mVert1.x);
	xmax = fmax(xmax, mVert2.x);

	double ymin = fmin(mVert0.y, mVert1.y);
	ymin = fmin(ymin, mVert2.y);

	double ymax = fmax(mVert0.y, mVert1.y);
	ymax = fmax(ymax, mVert2.y);

	double zmin = fmin(mVert0.z, mVert1.z);
	zmin = fmin(zmin, mVert2.z);

	double zmax = fmax(mVert0.z, mVert1.z);
	zmax = fmax(zmax, mVert2.z);

	output_box = aabb(
		point3(xmin, ymin, zmin),
		point3(xmax, ymax, zmax)
	);

	return true;
}