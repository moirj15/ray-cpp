#pragma once

#include "Scene.h"
#include "intersectData.h"

#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec4 ambient;

    Light(const glm::vec3 &p, const glm::vec4 &c, const glm::vec4 &a);
    ~Light(void);
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

    IlluminationModel() = default;
    IlluminationModel(f32 ambient_coef, f32 specular_coef, f32 diffuse_coef, f32 specular_exp, f32 reflection_const,
        f32 refraction_const);
    virtual ~IlluminationModel() = default;

    virtual glm::vec4 illuminate(IntersectData &id, const Light &light, const glm::vec3 &eyepoint, bool in_shadow) const = 0;
};

class Phong : public IlluminationModel
{
    glm::vec4 ambient_mat;
    glm::vec4 diffuse_mat;
    glm::vec4 specular_mat;

  public:
    Phong(void);
    Phong(f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, f32 t_const, glm::vec4 aM, glm::vec4 dM,
        glm::vec4 sM);
    ~Phong(void);

    glm::vec4 illuminate(IntersectData &id, const ::Light &light, const glm::vec3 &eyepoint, bool in_shadow) const;
};

class CheckerBoard : public IlluminationModel
{
    glm::vec4 color1;
    glm::vec4 color2;

  public:
    CheckerBoard(f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, f32 t_const, glm::vec4 &c1, glm::vec4 &c2);
    ~CheckerBoard(void);

    glm::vec4 illuminate(IntersectData &id, const ::Light &light, const glm::vec3 &eyepoint, bool in_shadow) const;

  private:
    glm::vec4 get_cube(IntersectData &id) const;
};