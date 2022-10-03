#pragma once

#include "../utils.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

struct Camera {
    glm::vec3 eyepoint;
    glm::vec3 look_at;
    glm::vec3 up;

    f32 film_plane_width;
    f32 film_plane_height;
    f32 f;

    const f32 ldmax = 1.0f;
    f32 lmax;

    glm::mat4 camera_transform;

    Camera(glm::vec3 e, glm::vec3 l, glm::vec3 u, f32 fp_w, f32 fp_h, f32 lm) :
        eyepoint(e), look_at(l), up(u), film_plane_width(fp_w), film_plane_height(fp_h), f(eyepoint.z),
        lmax(lm), camera_transform(glm::lookAt(eyepoint, look_at, up))
    {
    }
};

