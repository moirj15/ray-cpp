#include "scene.hpp"

#include "intersectData.h"
#include "ray.hpp"

Object *Scene::CastRay(const Ray &ray, IntersectData &data, s32 check_obj) const
{
    IntersectData closest_data;
    f32 closest_distance = INFINITY;
    f32 test_distance;
    s32 obj = -1;
    Object *hit = nullptr;
    for (const auto &object : m_objects) {

        IntersectData curr_data;

        if (object.get() != data.hit_obj && object->Intersect(ray, curr_data)) {
            test_distance = glm::distance(ray.origin, curr_data.intersection);
            //            if ((check_obj > -1) && (i == (u64)check_obj)) {
            //                continue;
            //            }
            if (test_distance <= closest_distance) {
                hit = object.get();
                closest_distance = test_distance;
                //                obj = (s32)i;
                curr_data.hit_obj = object.get();
                closest_data = curr_data;
            }
        }
    }
    data = closest_data;
    return hit;
}

bool Scene::InShadow(const IntersectData &intersect_data) const
{
    for (const auto &light : m_lights) {
//        const auto surfToLight = glm::normalize(light.position - intersect_data.intersection);
//        const auto viewVec = glm::normalize(-intersect_data.ray.direction);
//        const auto reflectionVec = glm::normalize(glm::reflect(-surfToLight, intersect_data.normal));
        const Ray ray(intersect_data.intersection, glm::normalize(light.position - intersect_data.intersection));
        IntersectData throw_away;
        for (const auto &object : m_objects) {
            if (object.get() != intersect_data.hit_obj && object->Intersect(ray, throw_away)) {
                return true;
            }
        }
    }
    return false;
}

void Scene::Transform(const glm::mat4 &mat)
{
    for (s32 i = 0; i < m_objects.size(); i++) {
        m_objects[i]->Transform(mat * m_object_transforms[i]);
    }
    for (auto &light : m_lights) {
        light.position = mat * glm::vec4(light.position, 1.0);
    }
}
