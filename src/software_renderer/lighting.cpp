#include "pipeline.h"


// TODO: move into pipeline.cpp

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>
#include <glm/gtx/string_cast.hpp>

namespace ra
{

/**
 * Lights the given triangle using the given light and material.
 *
 * @param triangle: The triangle that will be lit.
 * @param light: The light used to light the triangle.
 * @param mat: The triangle's material.
 * @return: The color of the triangle after lighting.
 */
u32 phong_light(std::vector<glm::vec4> triangle, const Light &light,
                const Material &mat)
{
    glm::vec4 normal(glm::triangleNormal(glm::vec3(triangle[0]),
                                           glm::vec3(triangle[1]),
                                         glm::vec3(triangle[2])), 0.0);
    normal = glm::normalize(normal);
    if (glm::dot(normal, glm::vec4(0.0f)) < 0.0f) {
        normal = -normal;
    }

	// TODO: lookup how to find position
    glm::vec4 pos = triangle[0];

    glm::vec4 surf_to_light = glm::normalize(light.pos - pos);

    // NOTE: normally this is camera.pos - pos, but the camera is at the origin
    glm::vec4 view_vec = glm::normalize(-pos);

    glm::vec4 reflection_vec = glm::normalize(glm::reflect(-surf_to_light , normal));
    reflection_vec.w = 0.0f;

    glm::vec4 ambient = light.ambient * mat.ambient_coef * mat.ambient_material;

    glm::vec4 diffuse = light.color * mat.diffuse_coef * mat.diffuse_material *
        fmaxf(0.0f, glm::dot(normal, surf_to_light));

    glm::vec4 specular = light.color * mat.specular_coef * mat.specular_material
        * powf(fmaxf(glm::dot(reflection_vec, view_vec), 0.0f), mat.specular_exp);

    glm::vec4 color = glm::clamp(ambient + diffuse + specular, glm::vec4(0.0f),
                                 glm::vec4(1.0f));
#if 0
    printf("%s\n", glm::to_string(pos).c_str());
    printf("%s\n", glm::to_string(ambient).c_str());
    printf("%s\n", glm::to_string(diffuse).c_str());
    printf("%s\n", glm::to_string(specular).c_str());
#endif

	u8 red = (u8) (255.0f * color.r);
	u8 green = (u8) (255.0f * color.g);
	u8 blue = (u8) (255.0f * color.b);
    //printf("r: %x\n", red);
    //printf("g: %x\n", green);
    //printf("b: %x\n", blue);

    return (u32) ((0xFF << 24) | (red << 16) | (green << 8) | blue);
    

}
}