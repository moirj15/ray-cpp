#pragma once

#include "../utils.hpp"
#include "object.hpp"
#include "shader.hpp"

#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

MAKE_HANDLE(ShaderHandle);
MAKE_HANDLE(TransformHandle);
MAKE_HANDLE(PositionHandle);
MAKE_HANDLE(GeometryHandle);
MAKE_HANDLE(ObjectHandle);

class Scene
{
public:
    struct Object {
        ShaderHandle   shader_handle;
        PositionHandle position_handle;
        GeometryHandle geometry_handle;
    };

private:
    std::unordered_map<GeometryHandle, std::unique_ptr<Geometry>> m_geometries;
    std::unordered_map<ShaderHandle, std::unique_ptr<Shader>>     m_shaders;
    std::unordered_map<PositionHandle, glm::vec3>                 m_positions;

    std::vector<Light>  m_lights;
    std::vector<Object> m_objects;

    static inline ShaderHandle   NEXT_SHADER_HANDLE;
    static inline PositionHandle NEXT_POSITION_HANDLE;
    static inline GeometryHandle NEXT_GEOMETRY_HANDLE;

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

    void AddObject(Geometry *geometry, const glm::vec3 &pos)
    {
        m_geometries.emplace(NEXT_GEOMETRY_HANDLE, geometry);
        NEXT_GEOMETRY_HANDLE++;

        m_positions.emplace(NEXT_POSITION_HANDLE, pos);
        NEXT_POSITION_HANDLE++;
    }

    Object GetObject(ObjectHandle handle) const { return m_objects[handle.GetValue()]; }

    Shader *GetShader(ShaderHandle handle) const { return m_shaders.at(handle).get(); }

    Geometry *GetGeometry(GeometryHandle handle) const { return m_geometries.at(handle).get(); }

    glm::vec3 GetPosition(PositionHandle handle) const { return m_positions.at(handle); }

    const std::unordered_map<GeometryHandle, std::unique_ptr<Geometry>> &GetGeometries() const { return m_geometries; }

    void                      AddLight(const Light &light) { m_lights.push_back(light); }
    const std::vector<Light> &GetLights() const { return m_lights; }

    ObjectHandle CastRay(const Ray &ray, SurfaceData &data, ObjectHandle last_hit) const;
    bool         InShadow(const SurfaceData &intersect_data, ObjectHandle last_hit) const;

    void Transform(const glm::mat4 &mat);
};
