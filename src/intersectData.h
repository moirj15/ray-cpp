#ifndef INTERSECTDATA_H
#define INTERSECTDATA_H

#include "utils.h"
#include "ray.h"

struct IntersectData {
	glm::vec3	intersection;
	glm::vec3	normal;
	f32			u_coord;
	f32			v_coord;
	glm::vec4 	triangle_points[3];
	Ray 		ray;

	IntersectData(void);
	IntersectData(const glm::vec3 &i, const glm::vec4 &n, f32 u, f32 v, Ray r);
	IntersectData(const glm::vec3 &i, const glm::vec4 &n, f32 u, f32 v, Ray r, glm::vec4 t_p[3]);

	~IntersectData(void);
};

#endif
