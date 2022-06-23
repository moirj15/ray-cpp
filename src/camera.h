#pragma once

#include "intersectData.h"
#include "scene.h"
#include "shader.h"
#include "utils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define WIDTH  600
#define HEIGHT 400

#define PIC_SIZE (WIDTH * HEIGHT)

enum TONE_TYPE { WARD, REINHARD };
class Frame
{
    const s32 m_width;
    const s32 m_height;
    std::vector<glm::vec3> m_image;

  public:
    Frame(s32 width, s32 height) : m_width(width), m_height(height), m_image(m_width * m_height, glm::vec4(0)) {}
    void SetPixel(s32 x, s32 y, const glm::vec3 &color) { m_image[m_width * y + x] = color; }
    void SetPixel(s32 i, const glm::vec3 &color) { m_image[i] = color; }
    [[nodiscard]] glm::vec3 GetPixel(s32 x, s32 y) const { return m_image[m_width * y + x]; }
    [[nodiscard]] glm::vec3 GetPixel(s32 i) const { return m_image[i]; }
    [[nodiscard]] s32 GetWidth() const { return m_width; }
    [[nodiscard]] s32 GetHeight() const { return m_height; }
    [[nodiscard]] const std::vector<glm::vec3> &GetImage() const { return m_image; }
};

struct Camera {
    glm::vec3 eyepoint;
    glm::vec3 look_at;
    glm::vec3 up;

    f32 film_plane_width;
    f32 film_plane_height;
    f32 f;

    const f32 ldmax = 1.0f;
    f32 lmax;

    glm::mat4 camera_transform;

    Camera(glm::vec3 e, glm::vec3 l, glm::vec3 u, f32 fp_w, f32 fp_h, f32 lm) :
            eyepoint(e), look_at(l), up(u), film_plane_width(fp_w), film_plane_height(fp_h), f(eyepoint.z),
            lmax(lm), camera_transform(glm::lookAt(eyepoint, look_at, up))
    {
    }
};

namespace tracer
{
void Render(const Scene &scene, const Camera &camera, Frame &frame);

//    s32 Intersection(const Scene &world, Ray ray, IntersectData &data, s32 check_obj);

glm::vec4 CalculateLight(
    const Scene &scene, const Camera &camera, IntersectData &id, const Shader &i_model, std::vector<Light> lights, s32 obj, u32 depth);

bool inShadow(const Scene &world, IntersectData &id, Light light, s32 obj);

void tone_rep(TONE_TYPE which_type);
} // namespace tracer
