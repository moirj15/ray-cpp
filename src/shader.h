#pragma once

#include "intersectData.h"

#include <glm/glm.hpp>

struct Scene;

struct Light {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec4 ambient;

    Light(const glm::vec3 &p, const glm::vec4 &c, const glm::vec4 &a) : position(p), color(c), ambient(a) {}
};

class Shader
{
  protected:
    f32 _ambient_coef;
    f32 _specular_coef;
    f32 _diffuse_coef;

    f32 _specular_exp;

    const Scene &_scene;

  public:
    f32 _reflection_const;
    f32 _refraction_const;

    Shader(const Scene &scene, f32 ambient_coef, f32 specular_coef, f32 diffuse_coef, f32 specular_exp,
        f32 reflection_const, f32 refraction_const) :
            _scene(scene),
            _ambient_coef(ambient_coef), _specular_coef(specular_coef), _diffuse_coef(diffuse_coef),
            _specular_exp(specular_exp), _reflection_const(reflection_const), _refraction_const(refraction_const)
    {
    }
    virtual ~Shader() = default;

    [[nodiscard]] virtual glm::vec3 Execute(const IntersectData &id) const = 0;
};

class Phong : public Shader
{
    glm::vec4 ambient_mat;
    glm::vec4 diffuse_mat;
    glm::vec4 specular_mat;

  public:
    Phong(const Scene &scene, f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, f32 t_const, glm::vec4 aM,
        glm::vec4 dM, glm::vec4 sM) :
            Shader(scene, a_coef, s_coef, d_coef, s_exp, r_const, t_const),
            ambient_mat(aM), diffuse_mat(dM), specular_mat(sM)
    {
    }

    [[nodiscard]] glm::vec3 Execute(const IntersectData &id) const override;
};

class CheckerBoard : public Shader
{
    glm::vec4 color1;
    glm::vec4 color2;

  public:
    CheckerBoard(const Scene &scene, f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, f32 t_const,
        const glm::vec4 &c1, const glm::vec4 &c2) :
            Shader(scene, a_coef, s_coef, d_coef, s_exp, r_const, t_const),
            color1(c1), color2(c2)
    {
    }

    [[nodiscard]] glm::vec3 Execute(const IntersectData &id) const override;

  private:
    glm::vec4 get_cube(const IntersectData &id) const;
};
