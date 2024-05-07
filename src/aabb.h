#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "ofMain.h"

using point3 = glm::vec3;

class aabb {
public:
    aabb() {}
    aabb(const point3& a, const point3& b) { minimum = a; maximum = b; }

    point3 min() const { return minimum; }
    point3 max() const { return maximum; }

    bool hit(const Ray& r, double t_min, double t_max) const;

    point3 minimum;
    point3 maximum;
};

aabb surrounding_box(aabb box0, aabb box1);

#endif

