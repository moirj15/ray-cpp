#include "scene.hpp"

#include "../intersectData.h"
#include "../math/ray.hpp"

ObjectHandle Scene::CastRay(const Ray &ray, SurfaceData &data, ObjectHandle last_hit) const
{
    SurfaceData  closest_data;
    f32          closest_distance = INFINITY;
    f32          test_distance;
    s32          obj = -1;
    ObjectHandle hit;
    for (u32 object_index = 0; object_index < m_objects.size(); object_index++) {

        SurfaceData curr_data;
        const Object &object = m_objects[object_index];


        if (object_index != last_hit && GetGeometry(object.geometry_handle)->Intersect(ray)) {
            test_distance = glm::distance(ray.origin, curr_data.pos);
            //            if ((check_obj > -1) && (i == (u64)check_obj)) {
            //                continue;
            //            }
            if (test_distance <= closest_distance) {
                hit              = ObjectHandle{object_index};
                closest_distance = test_distance;
                //                obj = (s32)i;
//                curr_data.hit_obj = object.get();
                closest_data      = curr_data;
            }
        }
    }
    data = closest_data;
    return hit;
}

bool Scene::InShadow(const SurfaceData &intersect_data, ObjectHandle last_hit) const
{
    for (const auto &light : m_lights) {
        //        const auto surfToLight = glm::normalize(light.position - intersect_data.intersection);
        //        const auto viewVec = glm::normalize(-intersect_data.ray.direction);
        //        const auto reflectionVec = glm::normalize(glm::reflect(-surfToLight, intersect_data.normal));
        const Ray     ray(intersect_data.pos, glm::normalize(light.position - intersect_data.pos));
        SurfaceData throw_away;
        for (u32 object_index = 0; object_index < m_objects.size(); object_index++) {
            const Object &object = m_objects[object_index];
            if (object_index != last_hit && GetGeometry(object.geometry_handle)->Intersect(ray)) {
                return true;
            }
        }
    }
    return false;
}

void Scene::Transform(const glm::mat4 &mat)
{
    for (s32 i = 0; i < m_geometries.size(); i++) {
        //m_geometries[i]->Transform(mat * m_transforms[i]);
    }
    for (auto &light : m_lights) {
        light.position = mat * glm::vec4(light.position, 1.0);
    }
}
