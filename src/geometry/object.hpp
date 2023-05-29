#pragma once

#include "../intersectData.h"
#include "../utils.hpp"

#include <glm/mat4x4.hpp>
#include <optional>
#include <vector>

struct Ray;
class Scene;
class Shader;

class Geometry
{
public:
    enum class Type {
        Sphere,
        Mesh,
    };
    const Type type;
    Geometry() = delete;
    explicit Geometry(Type type) : type(type) {}
    virtual ~Geometry() = default;

    virtual std::optional<SurfaceData> Intersect(const Ray &r) const         = 0;
    virtual void                       Transform(const glm::mat4 &transform) = 0;
};

class Sphere final : public Geometry
{
    f32       m_radius;
    glm::vec3 m_center;

public:
    Sphere(const glm::vec3 &c, const f32 r) : Geometry(Type::Mesh), m_radius(r), m_center(c) {}

    std::optional<SurfaceData> Intersect(const Ray &r) const override;
    void                       Transform(const glm::mat4 &transform) override;
};

class Mesh final : public Geometry
{
    std::vector<glm::vec3> m_vertices;
    std::vector<u32>       m_indices;

public:
    explicit Mesh(std::vector<glm::vec3> &&v, std::vector<u32> &&indices) : Geometry(Type::Mesh), m_vertices(v), m_indices(indices) {}
    Mesh(Mesh &&m) : Geometry(Type::Mesh), m_vertices(std::move(m.m_vertices)), m_indices(std::move(m.m_indices)) {}

    std::optional<SurfaceData> Intersect(const Ray &r) const override;
    void                       Transform(const glm::mat4 &transform) override;

    const std::vector<glm::vec3> &GetVertices() const { return m_vertices; }
    const std::vector<u32>       &GetIndices() const { return m_indices; }
};
