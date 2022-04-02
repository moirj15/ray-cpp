#pragma once

#include "utils.h"

#include <glm/vec4.hpp>

class Scene;
struct IntersectData;

class Material
{
  protected:
    const Scene &_scene;

  public:
    explicit Material(const Scene &scene) : _scene(scene) {}
    virtual ~Material() = default;

    [[nodiscard]] virtual glm::vec4 Sample(const IntersectData &intersect_data) const = 0;
};

class ReflectiveMaterial : public Material
{
    const f32 _reflection_const;

  public:
    ReflectiveMaterial(const Scene &scene, const f32 reflection_constant) :
            Material(scene), _reflection_const(reflection_constant)
    {
    }
    [[nodiscard]] glm::vec4 Sample(const IntersectData &intersect_data) const override;
};

class RefractiveMaterial : public Material
{
    const f32 _refraction_const;
    static constexpr f32 refract_air = 1.0f;
    static constexpr f32 refract_glass = 1.1f;

  public:
    RefractiveMaterial(const Scene &scene, const f32 refraction_constant) :
            Material(scene), _refraction_const(refraction_constant)
    {
    }
    [[nodiscard]] glm::vec4 Sample(const IntersectData &intersect_data) const override;
};
