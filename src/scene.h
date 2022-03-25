#pragma once

#include "object.h"
#include "illumination.h"

#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>

class Scene {
    std::vector<std::unique_ptr<Object>> objList;
    std::vector<Light> lightList;
  public:
    Scene() = default;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    Object *CastRay(const Ray &ray, IntersectData &data, s32 check_obj) const;

    void transform(glm::mat4 mat);
};