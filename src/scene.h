#pragma once

#include "object.h"
#include "illumination.h"

#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>

struct Scene {
    std::vector<std::unique_ptr<Object>> objList;
    std::vector<Light> lightList;

    Scene(void);
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;
    ~Scene(void);

    void transform(glm::mat4 mat);
};