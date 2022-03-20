#include "object.h"

#include <glm/gtx/projection.hpp>

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
bool Sphere::Intersect(const Ray &r, IntersectData &id)
{
    const glm::vec3 &d = r.direction;
    const glm::vec3 &o = r.origin;
    f32 B = 2.0f * (d.x * (o.x - _center.x) + d.y * (o.y - _center.y) + d.z * (o.z - _center.z));

    f32 C = pow(o.x - _center.x, 2.0f) + pow(o.y - _center.y, 2.0f) + pow(o.z - _center.z, 2.0f) - pow(_radius, 2.0f);

    // Calculate the value under the square root in the quadratic formula
    f32 squareRootCheck = pow(B, 2.0f) - (4.0f * C);
    if (squareRootCheck < 0.0) {
        return false;
    }
    // Calculate the quadratic formula
    f32 wAdd = (-B + sqrt(squareRootCheck)) / 2.0f;
    f32 wSub = (-B - sqrt(squareRootCheck)) / 2.0f;

    // determine the least square root
    f32 leastPos;

    if ((wAdd < 0.0) && (wSub < 0.0)) {
        return false;
    } else if (wSub < 0.0) {
        leastPos = wAdd;
    } else if (wAdd < 0.0) {
        leastPos = wSub;
    } else {
        leastPos = wAdd <= wSub ? wAdd : wSub;
    }

    // Calculate intersection and normal
    id.intersection = (o + (d * leastPos));
    id.normal = glm::normalize(id.intersection - _center);
    id.ray = r;
    return true;
}

void Sphere::Transform(const glm::mat4 &transform)
{
    _center = transform * glm::vec4(_center, 1.0f);
}


/**
 * Checks for the Intersection of an object with the given ray, if there is
 * an Intersection then the params intersection and normal will have their
 * contents modified.
 *
 * @param r: The ray that will be checked for Intersection.
 * @param intersection: The reference to the vector that will hold the
 * intersection point if there is an Intersection.
 * @param normal: The reference to the vector that will hold the surface
 * normal if there is an Intersection.
 * @return: True if the object was intersected, false otherwise.
 */
bool Polygon::Intersect(const Ray &r, IntersectData &id)
{
    for (u64 i = 0; i < _vertices.size(); i += 3) {
        // The _vertices of the triangle
        glm::vec3 p0 = glm::vec3(_vertices[i]);
        glm::vec3 p1 = glm::vec3(_vertices[i + 1]);
        glm::vec3 p2 = glm::vec3(_vertices[i + 2]);

        glm::vec3 e1 = p1 - p0;
        glm::vec3 e2 = p2 - p0;
        glm::vec3 T = glm::vec3(r.origin) - p0;
        glm::vec3 P = glm::cross(glm::vec3(r.direction), e2);
        glm::vec3 Q = glm::cross(T, e1);

        if (!glm::dot(P, e1)) {
            continue;
        }
        glm::vec3 tuv =
            (1 / (glm::dot(P, e1))) * glm::vec3(glm::dot(Q, e2), glm::dot(P, T), glm::dot(Q, glm::vec3(r.direction)));
        f32 t = tuv.x;
        f32 u = tuv.y;
        f32 v = tuv.z;
        if (t < 0.0) {
            continue;
        } else if ((u < 0.0) || (v < 0.0) || (u + v > 1.0)) {
            continue;
        }

        id.u_coord = u;
        id.v_coord = v;

        id.intersection = r.origin + t * r.direction;
        id.normal = glm::vec4(glm::cross(e1, e2), 0.0);
        id.triangle_points[0] = glm::vec4(p0, 1.0);
        id.triangle_points[1] = glm::vec4(p1, 1.0);
        id.triangle_points[2] = glm::vec4(p2, 1.0);
        if (id.normal.y < 0.0)
            id.normal = -id.normal;
        id.normal = glm::normalize(id.normal);
        id.ray = r;

        return true;
    }

    return false;
}

void Polygon::Transform(const glm::mat4 &transform)
{
    for (auto &vertex : _vertices) {
        vertex = transform * vertex;
    }
}
