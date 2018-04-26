#include "intersectData.h"
#include <math.h>

IntersectData::IntersectData(void) {
}

IntersectData::IntersectData(glm::vec4 i, glm::vec4 n, f32 u, f32 v, Ray r) :
		intersection(i), normal(n), u_coord(u), v_coord(v), ray(r)
{

}
IntersectData::IntersectData(glm::vec4 i, glm::vec4 n, f32 u, f32 v, Ray r, glm::vec4 t_p[3]) :
		intersection(i), normal(n), u_coord(u), v_coord(v), ray(r)
{
	triangle_points[0] = t_p[0];
	triangle_points[1] = t_p[1];
	triangle_points[2] = t_p[2];
}

IntersectData::~IntersectData(void) 
{

}
