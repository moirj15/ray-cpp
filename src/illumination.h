#pragma once

#include "intersectData.h"

#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec4 ambient;

    Light(const glm::vec3 &p, const glm::vec4 &c, const glm::vec4 &a) : position(p), color(c), ambient(a) {}
};

class IlluminationModel
{
  protected:
    f32 _ambient_coef;
    f32 _specular_coef;
    f32 _diffuse_coef;

    f32 _specular_exp;

  public:
    f32 _reflection_const;
    f32 _refraction_const;

    IlluminationModel(f32 ambient_coef, f32 specular_coef, f32 diffuse_coef, f32 specular_exp, f32 reflection_const,
        f32 refraction_const) :
            _ambient_coef(ambient_coef),
            _specular_coef(specular_coef), _diffuse_coef(diffuse_coef), _specular_exp(specular_exp),
            _reflection_const(reflection_const), _refraction_const(refraction_const)
    {
    }
    virtual ~IlluminationModel() = default;

    [[nodiscard]] virtual glm::vec4 illuminate(
        const IntersectData &id, const Light &light, const glm::vec3 &eyepoint, bool in_shadow) const = 0;
};

class Phong : public IlluminationModel
{
    glm::vec4 ambient_mat;
    glm::vec4 diffuse_mat;
    glm::vec4 specular_mat;

  public:
    Phong(f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, f32 t_const, glm::vec4 aM, glm::vec4 dM,
        glm::vec4 sM) :
            IlluminationModel(a_coef, s_coef, d_coef, s_exp, r_const, t_const),
            ambient_mat(aM), diffuse_mat(dM), specular_mat(sM)
    {
    }

    [[nodiscard]] glm::vec4 illuminate(
        const IntersectData &id, const ::Light &light, const glm::vec3 &eyepoint, bool in_shadow) const override;
};

class CheckerBoard : public IlluminationModel
{
    glm::vec4 color1;
    glm::vec4 color2;

  public:
    CheckerBoard(
        f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, f32 t_const, const glm::vec4 &c1, const glm::vec4 &c2) :
            IlluminationModel(a_coef, s_coef, d_coef, s_exp, r_const, t_const),
            color1(c1), color2(c2)
    {
    }

    [[nodiscard]] glm::vec4 illuminate(
        const IntersectData &id, const ::Light &light, const glm::vec3 &eyepoint, bool in_shadow) const override;

  private:
    glm::vec4 get_cube(const IntersectData &id) const;
};