#ifndef RAY_H
#define RAY_H

#include "utils.h"

// Ray object
struct Ray
{
	// NOTE: VECTORES HAVE TO BE NORMALIZED
	glm::vec4 origin;
	glm::vec4 direction;

	Ray(void);
	Ray(glm::vec4 o, glm::vec4 d);
	~Ray();
};
#endif
