#include "material.h"

#include "scene.h"

glm::vec4 ReflectiveMaterial::Sample(const IntersectData &intersect_data) const
{
#if 0
    const auto surfToLight = glm::normalize(_eyepoint - intersect_data.intersection);
    const auto reflectionVec = glm::normalize(-glm::reflect(surfToLight, intersect_data.normal));

    if (hit_obj > -1) {
        color += _shader._reflection_const
                 * CalculateLight(scene, data, scene.objList[hit_obj]->GetShader(), lights,
                                  hit_obj, depth + 1);
    } else {
        color += _shader._reflection_const * glm::vec4(0.0, 0.0, 1.0, 0.0);
    }
#endif
    return {};
}

glm::vec4 RefractiveMaterial::Sample(const IntersectData &intersect_data) const
{
#if 0
    f32 ni, nt;
    glm::vec3 normal, direction;
    const auto ray_dir = glm::normalize(intersect_data.ray.direction);
    if (glm::dot(intersect_data.normal, ray_dir) < 0.0) {
        ni = refract_glass;
        nt = refract_air;
        normal = (-intersect_data.normal);
    } else {
        ni = refract_air;
        nt = refract_glass;
        normal = (intersect_data.normal);
    }

    f32 internal_ref_check = 1.0f - ((powf(ni, 2.0f) * (1.0f - powf(glm::dot(ray_dir, normal), 2.0))) / powf(nt, 2.0f));

    if (internal_ref_check < 0.0) {
        // Total internal reflection
        direction = glm::normalize(glm::reflect(intersect_data.intersection, normal));
    } else {
        direction = glm::normalize(
            ((ni * (ray_dir - normal * glm::dot(ray_dir, normal))) / nt) + (normal * sqrtf(internal_ref_check)));
    }

    Ray transmission(intersect_data.intersection + (0.1f * normal), direction);
    IntersectData data;
    auto *hit_obj = _scene.CastRay(transmission, data, -1);
    glm::vec4 color(0);
    if (hit_obj) {
        color += _refraction_const * hit_obj->Sample(data);//CalculateLight(scene, data, hit_obj->GetShader(), lights, hit_obj, depth + 1);
    } else {
        color += _refraction_const * glm::vec4(0.0, 0.0, 1.0, 0.0);
    }
#endif
    return glm::vec4(0);
}
