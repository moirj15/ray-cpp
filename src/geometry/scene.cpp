#include "scene.hpp"

#include "../intersectData.h"
#include "../math/ray.hpp"

MeshHandle MeshManager::AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices)
{
    MeshHandle handle = s_next_handle;
    s_next_handle++;
    // TODO: replace this with a size check and a memcpy
    m_chunks.emplace_back(MeshChunk{
        .object_name = "",
        .start_vert  = m_vertices.size(),
        .vert_count  = vertices.size(),
        .start_index = m_indices.size(),
        .index_count = indices.size(),
    });
    for (const auto &v : vertices) {
        m_vertices.emplace_back(v);
    }
    for (const auto &i : indices) {
        m_indices.emplace_back(i);
    }
    return handle;
}

void MeshManager::Clear()
{
    m_vertices.clear();
    m_indices.clear();
    m_chunks.clear();
}

MeshHandle Scene::AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices) { return m_mesh_manager.AddMesh(vertices, indices); }

ObjectHandle Scene::CastRay(const Ray &ray, SurfaceData &data, ObjectHandle last_hit) const
{
    SurfaceData  closest_data;
    f32          closest_distance = INFINITY;
    f32          test_distance;
    s32          obj = -1;
    ObjectHandle hit = ObjectHandle::CreateInvalid();
    for (u32 object_index = 0; object_index < m_objects.size(); object_index++) {

        SurfaceData   curr_data;
        const Object &object = m_objects[object_index];
        assert(0); // TODO: rewrite this to use the new MeshManager

//        if (object_index != last_hit && GetGeometry(object.geometry_handle)->Intersect(ray)) {
//            test_distance = glm::distance(ray.origin, curr_data.pos);
//            //            if ((check_obj > -1) && (i == (u64)check_obj)) {
//            //                continue;
//            //            }
//            if (test_distance <= closest_distance) {
//                hit              = ObjectHandle{object_index};
//                closest_distance = test_distance;
//                //                obj = (s32)i;
//                //                curr_data.hit_obj = object.get();
//                closest_data = curr_data;
//            }
//        }
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
        assert(0); // TODO: rewrite this to use the new MeshManager
//        const Ray   ray(intersect_data.pos, glm::normalize(light.position - intersect_data.pos));
//        SurfaceData throw_away;
//        for (u32 object_index = 0; object_index < m_objects.size(); object_index++) {
//            const Object &object = m_objects[object_index];
//            if (object_index != last_hit && GetGeometry(object.geometry_handle)->Intersect(ray)) {
//                return true;
//            }
//        }
    }
    return false;
}

void Scene::Transform(const glm::mat4 &mat)
{
    for (s32 i = 0; i < m_geometries.size(); i++) {
        // m_geometries[i]->Transform(mat * m_transforms[i]);
    }
    for (auto &light : m_lights) {
        light.position = mat * glm::vec4(light.position, 1.0);
    }
}
