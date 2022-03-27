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
    for (const auto &object : objList) {

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

void Scene::transform(glm::mat4 mat)
{
    for (const auto &object : objList) {
        object->Transform(mat);
    }
    for (u64 l = 0; l < lightList.size(); l++) {
        lightList[l].position = mat * glm::vec4(lightList[l].position, 1.0);
    }
}
