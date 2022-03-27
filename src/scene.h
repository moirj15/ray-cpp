#pragma once

#include "illumination.h"
#include "object.h"

#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

class Scene
{
    std::vector<std::unique_ptr<Object>> objList;
    std::vector<Light> lightList;

  public:
    Scene() = default;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    void AddObject(Object *object) { objList.emplace_back(object); }
    void AddLight(const Light &light) { lightList.push_back(light); }
    const std::vector<Light> &GetLights() const { return lightList; }

    Object *CastRay(const Ray &ray, IntersectData &data, s32 check_obj) const;

    void transform(glm::mat4 mat);
};