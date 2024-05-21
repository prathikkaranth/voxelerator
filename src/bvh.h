//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#ifndef BVH_H
#define BVH_H

#include "hittable.h"
#include "hittable_list.h"

class bvh_node : public hittable {
public:
	bvh_node() = default;

	bvh_node(const hittable_list& list, double time0, double time1)
		: bvh_node(list.objects, 0, list.objects.size(), time0, time1)
	{}

	bvh_node(
		const std::vector<shared_ptr<hittable>>& src_objects,
		size_t start, size_t end, double time0, double time1);

	virtual bool hit(
		const Ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;



public:
	shared_ptr<hittable> left;
	shared_ptr<hittable> right;
	aabb box;
};

bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis);

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);

#endif

