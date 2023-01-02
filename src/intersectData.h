#ifndef INTERSECTDATA_H
#define INTERSECTDATA_H

// #include "ray.hpp"
#include "../utils.hpp"
#include "math/ray.hpp"
class Geometry;

struct SurfaceData {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 normal;
};

#endif
