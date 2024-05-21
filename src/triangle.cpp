#include "triangle.h"

bool Triangle::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {

	auto N = cross(mVert1 - mVert0, mVert2 - mVert0);
	N = normalize(N);
	const auto h = dot(-N, mVert0);
	const auto denom = dot(N, r.direction);
	if (denom < 1e-9f) return false;
	const auto t = -(dot(N, r.origin) + h) / denom;
	if (t < 0.0f) return false;

	const auto P = r.origin + t * r.direction;

	const auto C0 = cross(mVert1 - mVert0, P - mVert0);
	const auto C1 = cross(mVert2 - mVert1, P - mVert1);
	const auto C2 = cross(mVert0 - mVert2, P - mVert2);

	if (dot(N, C0) < 0.0f || dot(N, C1) < 0.0f || dot(N, C2) < 0.0f) return false;

	// Step 5: Find ray intersection
	rec.t = t;
	rec.p = P;
	rec.color = mColor;

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