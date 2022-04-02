#pragma once

#include "illumination.h"
#include "intersectData.h"
#include "utils.h"
#include "material.h"
#include <glm/mat4x4.hpp>

#include <vector>

struct Ray;
class Scene;

class Object {
protected:
    IlluminationModel *i_model;
    Material *_material;
    
public:
    Object() = delete;
    explicit Object(IlluminationModel *i, Material *material) :
            i_model(i), _material(material) {}
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
    [[nodiscard]] const IlluminationModel &GetIlluminationModel() const { return *i_model; }
    [[nodiscard]] glm::vec4 Sample(const IntersectData &intersect_data) { return i_model->illuminate(<#initializer #>) + _material->Sample(intersect_data); }
};

class Sphere final : public Object
{
    f32 _radius;
    glm::vec3 _center;

  public:
    Sphere(const glm::vec3 &c, const f32 r, IlluminationModel *i, Material *material) : Object(i, material),  _radius(r), _center(c) {}

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

class Polygon final : public Object
{
    // triangles will be stored as groups of 3 vec3s
    std::vector<glm::vec4> _vertices;
  public:

    /**
     * Constructor.
     *
     * @param v: The list of vertices, triangles will be in groups of 3.
     * @param n: The normal for the triangle face.
     */
    Polygon(std::vector<glm::vec4> &v, IlluminationModel *i, Material *material) : Object(i, material), _vertices(std::move(v))
    {
    }

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
};
