#pragma once

#include "intersectData.h"

#include <glm/glm.hpp>

class Scene;

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient;

    Light(const glm::vec3 &p, const glm::vec3 &c, const glm::vec3 &a) : position(p), color(c), ambient(a) {}
};

class Shader
{
  public:
    virtual ~Shader() = default;

    [[nodiscard]] virtual glm::vec3 Execute(const IntersectData &id) const = 0;
};

class FixedShader : public Shader
{
  protected:
    f32 m_ambient_coef;
    f32 m_specular_coef;
    f32 m_diffuse_coef;

    f32 m_specular_exp;

    const Scene &m_scene;

  public:
    f32 m_reflection_const;
    f32 m_refraction_const;

    FixedShader(const Scene &scene, f32 ambient_coef, f32 specular_coef, f32 diffuse_coef, f32 specular_exp,
        f32 reflection_const, f32 refraction_const) :
            m_scene(scene),
            m_ambient_coef(ambient_coef), m_specular_coef(specular_coef), m_diffuse_coef(diffuse_coef),
            m_specular_exp(specular_exp), m_reflection_const(reflection_const), m_refraction_const(refraction_const)
    {
    }
    ~FixedShader() override = default;

//    [[nodiscard]] virtual glm::vec3 Execute(const IntersectData &id) const = 0;
};

class Phong : public FixedShader
{
    glm::vec3 m_ambient_mat;
    glm::vec3 m_diffuse_mat;
    glm::vec3 m_specular_mat;

  public:
    Phong(const Scene &scene, f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, f32 t_const, glm::vec4 aM,
        glm::vec4 dM, glm::vec4 sM) :
            FixedShader(scene, a_coef, s_coef, d_coef, s_exp, r_const, t_const),
            m_ambient_mat(aM), m_diffuse_mat(dM), m_specular_mat(sM)
    {
    }

    [[nodiscard]] glm::vec3 Execute(const IntersectData &id) const override;
};

class CheckerBoard : public FixedShader
{
    glm::vec4 m_color1;
    glm::vec4 m_color2;

  public:
    CheckerBoard(const Scene &scene, f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, f32 t_const,
        const glm::vec4 &c1, const glm::vec4 &c2) :
            FixedShader(scene, a_coef, s_coef, d_coef, s_exp, r_const, t_const),
            m_color1(c1), m_color2(c2)
    {
    }

    [[nodiscard]] glm::vec3 Execute(const IntersectData &id) const override;

  private:
    glm::vec4 get_cube(const IntersectData &id) const;
};
