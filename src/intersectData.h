#ifndef INTERSECTDATA_H
#define INTERSECTDATA_H

#include "ray.hpp"
#include "utils.hpp"
class Object ;

struct IntersectData {
    glm::vec3 intersection{0, 0, 0};
    glm::vec3 normal{0, 0, 0};
    f32 u_coord = 0.0f;
    f32 v_coord = 0.0f;
    glm::vec4 triangle_points[3];
    Ray ray;
    Object *hit_obj = nullptr; // temp hack to protect against self intersections when casting reflection ray

    IntersectData() = default;
    IntersectData(const glm::vec3 &i, const glm::vec4 &n, f32 u, f32 v, Ray r) :
        intersection(i), normal(n), u_coord(u), v_coord(v), ray(r)
    {

    }
    IntersectData(const glm::vec3 &i, const glm::vec4 &n, f32 u, f32 v, Ray r, glm::vec4 t_p[3]) :
        intersection(i), normal(n), u_coord(u), v_coord(v), ray(r)
    {
        triangle_points[0] = t_p[0];
        triangle_points[1] = t_p[1];
        triangle_points[2] = t_p[2];
    }
};

#endif
