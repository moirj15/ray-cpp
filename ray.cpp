#include "ray.h"

Ray::Ray(void)
{

}

Ray::Ray(glm::vec4 o, glm::vec4 d)
	: origin(o), direction(d)
{

}

Ray::~Ray()
{

}
