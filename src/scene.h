#pragma once

#include "object.h"
#include "shader.h"

#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

class Scene
{
    std::vector<std::unique_ptr<Object>> _objects;
    std::vector<Light> _lights;

  public:
    Scene() = default;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    void AddObject(Object *object) { _objects.emplace_back(object); }
    void AddLight(const Light &light) { _lights.push_back(light); }
    [[nodiscard]] const std::vector<Light> &GetLights() const { return _lights; }

    [[nodiscard]] Object *CastRay(const Ray &ray, IntersectData &data, s32 check_obj) const;
    [[nodiscard]] bool InShadow(const IntersectData &intersect_data) const;

    void Transform(glm::mat4 mat);
};
