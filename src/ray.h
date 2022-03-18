#ifndef RAY_H
#define RAY_H

#include "utils.h"

// Ray object
struct Ray
{
	// NOTE: VECTORES HAVE TO BE NORMALIZED
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(void);
	Ray(const glm::vec3 &o, const glm::vec3 &d);
	~Ray();
};
#endif
