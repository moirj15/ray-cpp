#pragma once

#include "object.h"
#include "shader.h"

#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

class Scene
{
    std::vector<std::unique_ptr<Object>> m_objects;
    std::vector<glm::mat4> m_object_transforms;
    std::vector<Light> m_lights;

  public:
    Scene() = default;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    void AddObject(Object *object, const glm::mat4 &transform = glm::mat4(1.0)) {
        m_objects.emplace_back(object);
        m_object_transforms.emplace_back(transform);
    }
    [[nodiscard]] const std::vector<std::unique_ptr<Object>> &GetObjects() const { return m_objects; }
    void AddLight(const Light &light) { m_lights.push_back(light); }
    [[nodiscard]] const std::vector<Light> &GetLights() const { return m_lights; }

    [[nodiscard]] Object *CastRay(const Ray &ray, IntersectData &data, s32 check_obj) const;
    [[nodiscard]] bool InShadow(const IntersectData &intersect_data) const;

    void Transform(const glm::mat4 &mat);
};
