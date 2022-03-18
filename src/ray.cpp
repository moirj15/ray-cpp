#include "ray.h"

Ray::Ray(void)
{

}

Ray::Ray(const glm::vec3 &o, const glm::vec3 &d)
	: origin(o), direction(d)
{

}

Ray::~Ray()
{

}
