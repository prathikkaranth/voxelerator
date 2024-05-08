#include "triangle.h"

//bool Triangle::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const{
//
//	glm::vec3 edge1 = mVert1 - mVert0;
//	glm::vec3 edge2 = mVert2 - mVert0;
//	glm::vec3 h = glm::cross(r.direction, edge2);
//	float det = glm::dot(edge1, h);
//
//	if (det > -0.00001 && det < 0.00001) return false;
//
//	float invDet = 1.0 / det;
//	glm::vec3 s = r.origin - mVert0;
//	float u = invDet * glm::dot(s, h);
//
//	if (u < 0 || u > 1) return false;
//
//	glm::vec3 q = glm::cross(s, edge1);
//	float v = invDet * glm::dot(r.direction, q);
//
//	if (v < 0 || u + v > 1) return false;
//
//	float t = invDet * glm::dot(edge2, q);
//	rec.t = t;
//
//	if (t > 0.00001) {
//		return true;
//	}
//
//	return false;
//}

bool Triangle::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {

	// Computer the plane's normal
	glm::vec3 v0v1 = mVert1 - mVert0;
	glm::vec3 v0v2 = mVert2 - mVert0;

	glm::vec3 n = cross(v0v1, v0v2);
	float area2 = n.length();

	// Step 1: Finding P

	// check if ray and plane are parallel ?
	float NdotRayDirection = dot(n, r.direction);
	if (fabs(NdotRayDirection) < 1e-12) // almost 0 
		return false; // they are parallel so they don't intersect !

	// compute d parameter using equation 2
	float d = dot(-n, mVert0);

	// compute t (equation 3)
	float t = -(dot(n, r.origin) + d) / NdotRayDirection;

	// check if the triangle is in behind the ray
	if (t < 0) return false; // the triangle is behind

	// compute the intersection point using equation 1
	glm::vec3 P = r.origin + t * r.direction;

	// Step 2: Inside-outside test
	glm::vec3 C; // vector perpendicular to triangle's plane

	// edge 0
	glm::vec3 edge0 = mVert1 - mVert0;
	glm::vec3 vp0 = P - mVert0;
	C = cross(edge0, vp0);
	if (dot(n, C) < 0) return false; // P is on the right side

	// edge 1
	glm::vec3 edge1 = mVert2 - mVert1;
	glm::vec3 vp1 = P - mVert1;
	C = cross(edge1, vp1);
	if (dot(n, C) < 0)  return false; // P is on the right side

	// edge 2
	glm::vec3 edge2 = mVert0 - mVert2;
	glm::vec3 vp2 = P - mVert2;
	C = cross(edge2, vp2);
	if (dot(n, C) < 0) return false; // P is on the right side;

	// Step 5: Find ray intersection
	rec.t = t;
	rec.p = P;

	return true;
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