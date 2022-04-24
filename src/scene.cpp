#include "scene.h"

#include "intersectData.h"
#include "ray.h"

Object *Scene::CastRay(const Ray &ray, IntersectData &data, s32 check_obj) const
{
    IntersectData closest_data;
    f32 closest_distance = INFINITY;
    f32 test_distance;
    s32 obj = -1;
    Object *hit = nullptr;
    for (const auto &object : _objects) {

        IntersectData curr_data;

        if (object->Intersect(ray, curr_data)) {
            test_distance = glm::distance(ray.origin, curr_data.intersection);
//            if ((check_obj > -1) && (i == (u64)check_obj)) {
//                continue;
//            }
            if (test_distance <= closest_distance) {
                hit = object.get();
                closest_distance = test_distance;
//                obj = (s32)i;
                closest_data = curr_data;
            }
        }
    }
    data = closest_data;
    return hit;
}

bool Scene::InShadow(const IntersectData &intersect_data) const
{
    for (const auto &light : _lights) {
        const auto surfToLight = glm::normalize(light.position - intersect_data.intersection);
        const auto viewVec = glm::normalize(-intersect_data.ray.direction);
        const auto reflectionVec = glm::normalize(glm::reflect(-surfToLight, intersect_data.normal));
        const Ray ray(intersect_data.intersection, reflectionVec);
        IntersectData throw_away;
        for (const auto &object : _objects) {
            if (object->Intersect(ray, throw_away)) {
                return true;
            }
        }
    }
    return false;
}

void Scene::Transform(glm::mat4 mat)
{
    for (const auto &object : _objects) {
        object->Transform(mat);
    }
    for (auto &light : _lights) {
        light.position = mat * glm::vec4(light.position, 1.0);
    }
}
