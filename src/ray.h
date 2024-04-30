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
