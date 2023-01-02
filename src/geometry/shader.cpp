#include "shader.hpp"

#include "scene.hpp"

glm::vec3 Phong::Execute(const SurfaceData &id) const
{
    #if 0
    glm::vec3 ret_col(0, 0, 0);
    for (const auto &light : m_scene.GetLights()) {
        const glm::vec3 surfToLight = glm::normalize(light.position - id.pos);
        const glm::vec3 viewVec = glm::normalize(-id.ray.direction);
        const glm::vec3 reflectionVec = glm::normalize(glm::reflect(-surfToLight, id.normal));

        glm::vec3 ambient, diffuse, specular;

        ambient = light.ambient * m_ambient_coef * m_ambient_mat;
        if (m_scene.InShadow(id)) {
            ret_col = ambient;
        } else {
            diffuse = light.color * m_diffuse_coef * m_diffuse_mat * fmaxf(0.0, glm::dot(id.normal, surfToLight));
            specular = light.color * m_specular_coef * m_specular_mat
                       * powf(fmaxf(glm::dot(reflectionVec, viewVec), 0.0), m_specular_exp);
            ret_col = ambient + diffuse + specular;
        }
    }
    
    return ret_col;
    #endif
    return {};
}

glm::vec3 CheckerBoard::Execute(const SurfaceData &surface_data) const
{
#if 0
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
#endif
    return {0, 0, 0};
}

glm::vec4 CheckerBoard::get_cube(const SurfaceData &surface_data) const
{
    f32 u_barry = surface_data.uv.x;
    f32 v_barry = surface_data.uv.y;
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
        return m_color2;
    } else if (!row_odd && col_odd) {
        return m_color1;
    } else if (row_odd && !col_odd) {
        return m_color1;
    } else { //(!row_odd && !col_odd)
        return m_color2;
    }
}
