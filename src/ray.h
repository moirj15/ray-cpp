#ifndef RAY_H
#define RAY_H

#include "utils.h"

// Ray object
struct Ray {
    // NOTE: VECTORS HAVE TO BE NORMALIZED
    glm::vec3 origin;
    glm::vec3 direction;

    Ray() = default;
    Ray(const glm::vec3 &o, const glm::vec3 &d) : origin(o), direction(d) {}
};
#endif
