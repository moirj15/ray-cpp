#pragma once

#include "../utils.hpp"
#include "object.hpp"
#include "shader.hpp"

#include <glm/mat4x4.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

MAKE_HANDLE(ShaderHandle);
MAKE_HANDLE(TransformHandle);
MAKE_HANDLE(PositionHandle);
MAKE_HANDLE(GeometryHandle);
MAKE_HANDLE(MeshHandle);
MAKE_HANDLE(ObjectHandle);

struct MeshChunk {
    const std::string object_name;
    const u64         start_vert;
    const u64         vert_count;
    const u64         start_index;
    const u64         index_count;
};
class MeshManager
{
private:
    std::vector<glm::vec3>   m_vertices;
    std::vector<u32>         m_indices; // TODO: Switch to collection of 8, 16, and 32 bit integers
    std::vector<MeshChunk>   m_chunks;
    inline static MeshHandle s_next_handle{0};

public:
    MeshHandle AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices);

    const std::vector<glm::vec3> &GetVertices() const { return m_vertices; }
    const std::vector<u32>       &GetIndices() const { return m_indices; }
    const std::vector<MeshChunk> &GetMeshChunks() const { return m_chunks; }

    void Clear();
};

class Scene
{
public:
    struct Object {
        ShaderHandle   shader_handle;
        PositionHandle position_handle;
        //        GeometryHandle geometry_handle;
        MeshHandle mesh_handle;
    };

private:
    MeshManager m_mesh_manager;

    std::unordered_map<GeometryHandle, std::unique_ptr<Geometry>> m_geometries;
    std::unordered_map<ShaderHandle, std::unique_ptr<Shader>>     m_shaders;
    std::unordered_map<PositionHandle, glm::vec3>                 m_positions;

    std::vector<Light>  m_lights;
    std::vector<Object> m_objects;

    static inline ShaderHandle   NEXT_SHADER_HANDLE{0};
    static inline PositionHandle NEXT_POSITION_HANDLE{0};
    static inline GeometryHandle NEXT_GEOMETRY_HANDLE{0};

public:
    Scene()                         = default;
    Scene(const Scene &)            = delete;
    Scene &operator=(const Scene &) = delete;
    Scene(Scene &&s) noexcept :
        m_geometries(std::move(s.m_geometries)),
        m_positions(std::move(s.m_positions)),
        m_lights(std::move(s.m_lights)),
        m_objects(std::move(s.m_objects))
    {
    }

    MeshHandle AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices);

    //    void AddObject(Geometry *geometry, const glm::vec3 &pos)
    void AddObject(MeshHandle mesh_handle, ShaderHandle shader_handle, const glm::vec3 &pos)
    {
        //        m_geometries.emplace(NEXT_GEOMETRY_HANDLE, geometry);
        //        NEXT_GEOMETRY_HANDLE++;

        m_positions.emplace(NEXT_POSITION_HANDLE, pos);

        m_objects.emplace_back(Object{
            .shader_handle   = shader_handle,
            .position_handle = NEXT_POSITION_HANDLE,
            .mesh_handle     = mesh_handle,
        });
        NEXT_POSITION_HANDLE++;
    }

    ShaderHandle AddShader(Shader *shader)
    {
        auto handle = NEXT_SHADER_HANDLE;
        m_shaders.emplace(handle, shader);
        NEXT_SHADER_HANDLE++;
        return handle;
    }

    Object GetObject(ObjectHandle handle) const { return m_objects[handle.GetValue()]; }

    Shader *GetShader(ShaderHandle handle) const { return m_shaders.at(handle).get(); }

    const MeshManager &GetMeshManager() const { return m_mesh_manager; }

//    Geometry *GetGeometry(GeometryHandle handle) const { return m_geometries.at(handle).get(); }

    glm::vec3 GetPosition(PositionHandle handle) const { return m_positions.at(handle); }

//    const std::unordered_map<GeometryHandle, std::unique_ptr<Geometry>> &GetGeometries() const { return m_geometries; }

    void                      AddLight(const Light &light) { m_lights.push_back(light); }
    const std::vector<Light> &GetLights() const { return m_lights; }

    ObjectHandle CastRay(const Ray &ray, SurfaceData &data, ObjectHandle last_hit) const;
    bool         InShadow(const SurfaceData &intersect_data, ObjectHandle last_hit) const;

    void Transform(const glm::mat4 &mat);
};
