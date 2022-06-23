#pragma once

#include "intersectData.h"
#include "material.h"
#include "shader.h"
#include "utils.h"

#include <glm/mat4x4.hpp>
#include <vector>

struct Ray;
class Scene;

class Object
{
protected:
    const Shader *m_shader;

public:
    enum class Type {
        Sphere,
        Mesh,
    };
    const Type type;
    Object() = delete;
    explicit Object(Shader *shader, Type type) : m_shader(shader), type(type) {}
    /**
     * Virtual destructor.
     */
    virtual ~Object() = default;

    /**
     * Checks for the Intersection of an object with the given ray, if there is
     * an intersection then the params Intersection and normal will have their
     * contents modified.
     *
     * @param r: The ray that will be checked for Intersection.
     * @param intersection: The reference to the vector that will hold the
     * intersection point if there is an Intersection.
     * @param normal: The reference to the vector that will hold the surface
     * normal if there is an Intersection.
     * @return: True if the object was intersected, false otherwise.
     */
    virtual bool Intersect(const Ray &r, IntersectData &id) const = 0;
    virtual void Transform(const glm::mat4 &transform) = 0;
    [[nodiscard]] const Shader &GetShader() const { return *m_shader; }
    //    [[nodiscard]] glm::vec4 Sample(const IntersectData &intersect_data)
    //    {
    //        return _shader->Execute(intersect_data) + _material->Sample(intersect_data);
    //    }
};

class Sphere final : public Object
{
    f32 m_radius;
    glm::vec3 m_center;

public:
    Sphere(const glm::vec3 &c, const f32 r, Shader *i) : Object(i, Type::Mesh), m_radius(r), m_center(c) {}

    /**
     * Checks for the Intersection of an object with the given ray, if there is
     * an intersection then the params Intersection and normal will have their
     * contents modified.
     *
     * @param r: The ray that will be checked for Intersection.
     * @param intersection: The reference to the vector that will hold the
     * intersection point if there is an Intersection.
     * @param normal: The reference to the vector that will hold the surface
     * normal if there is an Intersection.
     * @return: True if the object was intersected, false otherwise.
     */
    bool Intersect(const Ray &r, IntersectData &id) const override;
    void Transform(const glm::mat4 &transform) override;
};

class Mesh final : public Object
{
    std::vector<glm::vec3> m_vertices;
    std::vector<u32> m_indices;

public:
    Mesh(std::vector<glm::vec3> &v, std::vector<u32> indices, Shader *i) : Object(i, Type::Mesh), m_vertices(std::move(v)), m_indices(std::move(indices)) {}

    /**
     * Checks for the Intersection of an object with the given ray, if there is
     * an intersection then the params Intersection and normal will have their
     * contents modified.
     *
     * @param r: The ray that will be checked for Intersection.
     * @param intersection: The reference to the vector that will hold the
     * Intersection point if there is an intersection.
     * @param normal: The reference to the vector that will hold the surface
     * normal if there is an Intersection.
     * @return: True if the object was intersected, false otherwise.
     */
    bool Intersect(const Ray &r, IntersectData &id) const override;
    void Transform(const glm::mat4 &transform) override;

    [[nodiscard]] const std::vector<glm::vec3> GetVertices() const { return m_vertices; }
};
