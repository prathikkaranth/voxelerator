#include "ofMain.h"
#include "ray.h"
#include "aabb.h"
#include "bvh.h"
#include "hittable.h"

class Triangle : public hittable {
	public:
		Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
			mVert0 = v0;
			mVert1 = v1;
			mVert2 = v2;
		}
		
		virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

		virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

		glm::vec3 mVert0;
		glm::vec3 mVert1;
		glm::vec3 mVert2;

};


