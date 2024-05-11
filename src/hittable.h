#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"

class material;

struct hit_record {
    point3 p;
    glm::vec3 normal;

    ofFloatColor color;
    double t;
 
};

class hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;

};

#endif
