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

#ifndef _RAY_H_
#define _RAY_H_

#include "ofMain.h"

class Ray {
  public:
	  Ray(glm::vec3 o, glm::vec3 d) {
		  origin = o;
		  direction = d;
		  inv_direction = glm::vec3(1 / d.x, 1 / d.y, 1 / d.z);
		  sign[0] = (inv_direction.x < 0);
		  sign[1] = (inv_direction.y < 0);
		  sign[2] = (inv_direction.z < 0);
	  }
	  Ray(const Ray& r) {
		  origin = r.origin;
		  direction = r.direction;
		  inv_direction = r.inv_direction;
		  sign[0] = r.sign[0]; sign[1] = r.sign[1]; sign[2] = r.sign[2];
	  }
	  glm::vec3 origin;
	  glm::vec3 direction;
	  glm::vec3 inv_direction;
	  int sign[3];
};


#endif // _RAY_H_
