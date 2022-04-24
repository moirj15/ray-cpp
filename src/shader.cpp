#include "shader.h"
#include "scene.h"

glm::vec3 Phong::Execute(const IntersectData &id) const
{
    for (const auto &light : _scene.GetLights()) {
        const auto surfToLight = glm::normalize(light.position - id.intersection);
        const auto viewVec = glm::normalize(-id.ray.direction);
        const auto reflectionVec = glm::normalize(glm::reflect(-surfToLight, id.normal));

        glm::vec4 ambient, diffuse, specular;
        glm::vec4 ret_col;

        ambient = light.ambient * _ambient_coef * ambient_mat;
        if (_scene.InShadow(id)) {
            return ambient;
        } else {
            diffuse = light.color * _diffuse_coef * diffuse_mat * fmaxf(0.0, glm::dot(id.normal, surfToLight));
            specular = light.color * _specular_coef * specular_mat
                       * powf(fmaxf(glm::dot(reflectionVec, viewVec), 0.0), _specular_exp);
            return ambient + diffuse + specular;
        }
    }
}

glm::vec3 CheckerBoard::Execute(const IntersectData &id) const
{
    const auto surfToLight = glm::normalize(light.position - id.intersection);
    const auto viewVec = glm::normalize(id.ray.origin - id.intersection);
    const auto reflectionVec = glm::normalize(glm::reflect(-surfToLight, id.normal));

    glm::vec4 ambient, diffuse, specular;

    ambient = light.ambient * _ambient_coef * get_cube(id);

    if (in_shadow) {
        return ambient;
    } else {
        diffuse = light.color * _diffuse_coef * get_cube(id) * fmaxf(0.0, glm::dot(id.normal, surfToLight));

        specular = light.color * _specular_coef * get_cube(id)
                   * powf(fmaxf(glm::dot(reflectionVec, viewVec), 0.0), _specular_exp);

        return ambient + diffuse + specular;
    }
}

glm::vec4 CheckerBoard::get_cube(const IntersectData &id) const
{
    f32 u_barry = id.u_coord;
    f32 v_barry = id.v_coord;
    f32 w_barry = 1 - u_barry - v_barry;
    glm::vec4 A(10.0, 1.0, 0.0, 0.0); // = id.triangle_points[0];
    glm::vec4 B(0.0, 1.0, 0.0, 0.0);  //= id.triangle_points[1];
    glm::vec4 C(0.0, 1.0, 10.0, 0.0); //= id.triangle_points[2];

    f32 u = (u_barry * A + v_barry * B + w_barry * C).x;
    f32 v = (u_barry * A + v_barry * B + w_barry * C).y;

    // printf("%f %f %f\n", u_barry, v_barry, w_barry);
    u32 row_odd = ((u32)(u_barry * 12.0)) % 2;
    u32 col_odd = ((u32)(v_barry * 12.0)) % 2;
    // printf("row %d col %d\n", row_odd , col_odd);
    if (!row_odd && !col_odd) {
        return color2;
    } else if (!row_odd && col_odd) {
        return color1;
    } else if (row_odd && !col_odd) {
        return color1;
    } else { //(!row_odd && !col_odd)
        return color2;
    }
}
