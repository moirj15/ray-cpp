#pragma once
#include "canvas.h"
#include "utils.h"
namespace ra
{
struct Camera {
    glm::vec3 eyePoint;
    glm::vec3 lookAt;
    glm::vec3 upVec;

    glm::mat4 projection;
    glm::mat4 cameraMat;
    glm::mat4 modelMat;

    /**
     * Constructor.
     */
    Camera(void);

    /**
     * Constructor.
     *
     * @param e: The eyepoint.
     * @param l: The look-up vector.
     * @param u: The up vector.
     * @param width: Width of the viewing plane.
     * @param height: Height of the viewing plane.
     * @param fov: Field of view.
     * @param near: Near plane z-value.
     * @param far: Far plane z-value.
     */
    Camera(glm::vec3 e, glm::vec3 l, glm::vec3 u, u32 width, u32 height, f32 fov, f32 near, f32 far) : eyePoint(e), lookAt(l), upVec(u)
    {
        projection = glm::perspective(fov, (f32)width / (f32)height, near, far);
        cameraMat  = glm::lookAt(eyePoint, eyePoint + lookAt, upVec);
        modelMat   = glm::mat4(1.0f); // no model transforms for now
    }

    ~Camera(void)
    {
    }
};

struct VBO {
    std::vector<glm::vec4> vertexList;
    std::vector<u64>       connections;

    VBO(void){};
    VBO(std::vector<glm::vec4> v, std::vector<u64> c) : vertexList(v), connections(c){};

    ~VBO(void){};
};

struct Light {
    glm::vec4 pos;
    glm::vec4 color;
    glm::vec4 ambient;
};

struct Material {
    glm::vec4 ambient_material;
    glm::vec4 diffuse_material;
    glm::vec4 specular_material;

    f32 ambient_coef;
    f32 diffuse_coef;
    f32 specular_coef;
    f32 specular_exp;
};

// @IMPL: lighting.cpp
/**
 * Lights the given triangle using the given light and material.
 *
 * @param triangle: The triangle that will be lit.
 * @param light: The light used to light the triangle.
 * @param mat: The triangle's material.
 * @return: The color of the triangle after lighting.
 */
u32 phong_light(std::vector<glm::vec4> triangle, const Light &light, const Material &mat);

// @IMPL: triangulation.cpp
/**
 * Converts the given polygon into a VBO filled with triangles.
 *
 * @param polygon: The polygon that will be converted.
 * @return: The VBO containing the polygons.
 */
VBO triangulate(std::vector<glm::vec4> polygon);

// @IMPL: transform.cpp
/**
 * Transforms the given VBO into a set of 2D triangles and lights them.
 *
 * @param object: The VBO that will be transformed.
 * @param camera: The camera viewing the object.
 * @param light: The scene's light.
 * @param mat: The material of the object.
 * @param colors: The list of colors that will be filled after lighting the
 * triangles.
 * @return: The list of 2D triangles.
 */
VBO transform_to_2D(VBO object, Camera *camera, const Light &light, const Material &mat, std::vector<u32> &colors);

// @IMPL: clipper.cpp
/**
 * Clips the given triangle to the given bounding box.
 *
 * @param poly: The polygon that will be clipped.
 * @param ll: The lower left vertex of the bounding box.
 * @param ur: The upper right vertex of the bounding box.
 * @return: The clipped polygon.
 */
std::vector<glm::vec4> clipPolygon(std::vector<glm::vec4> &poly, glm::vec2 ll, glm::vec2 ur);

// @IMPL: triangleRasterizer.cpp
/**
 * Draws the given VBO to the given canvas.
 *
 * @param vbo: The vbo that will be drawn to the canvas.
 * @param can: The canvas that will be drawn to.
 * @param values: The color values for each triangle.
 */
void draw_vbo(VBO vbo, Canvas *can, std::vector<u32> values);

#if 0
// @IMPL: rasterizer.cpp
/**
 * Draws the given polygon, with the given color, onto the given canvas.
 *
 * @param poly: The polygon that will be drawn.
 * @param can: The canvas that the polygon will be drawn to.
 * @param value: The value that the polygon will be colored with.
 */
void fillPolygon(std::vector<glm::vec2> poly, Canvas *can, u32 value);
#endif
} // namespace ra