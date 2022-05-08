#pragma once

#include "intersectData.h"
#include "scene.h"
#include "shader.h"
#include "utils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define WIDTH 600
#define HEIGHT 400

#define PIC_SIZE (WIDTH * HEIGHT)

enum TONE_TYPE { WARD, REINHARD };
class Frame
{
    const s32 _width;
    const s32 _height;
    std::vector<glm::vec3> _image;

  public:
    Frame(s32 width, s32 height) : _width(width), _height(height), _image(_width * _height, glm::vec4(0)) {}
    void SetPixel(s32 x, s32 y, const glm::vec3 &color) { _image[_width * y + x] = color; }
    void SetPixel(s32 i, const glm::vec3 &color) { _image[i] = color; }
    [[nodiscard]] glm::vec3 GetPixel(s32 x, s32 y) const { return _image[_width * y + x]; }
    [[nodiscard]] glm::vec3 GetPixel(s32 i) const { return _image[i]; }
    [[nodiscard]] s32 GetWidth() const { return _width; }
    [[nodiscard]] s32 GetHeight() const { return _height; }
    [[nodiscard]] const std::vector<glm::vec3> &GetImage() const { return _image; }
};

class Camera
{
    glm::vec3 _eyepoint;
    glm::vec3 _lookAt;
    glm::vec3 _up;

    f32 _film_plane_width;
    f32 _film_plane_height;
    f32 _f;

    const f32 ldmax = 1.0f;
    f32 lmax;

    Frame _frame;
    glm::mat4 _camera_transform;

  public:
    Camera(glm::vec3 e, glm::vec3 l, glm::vec3 u, f32 fp_w, f32 fp_h, f32 lm) :
            _eyepoint(e), _lookAt(l), _up(u), _film_plane_width(fp_w), _film_plane_height(fp_h), _f(_eyepoint.z),
            lmax(lm), _frame(WIDTH, HEIGHT), _camera_transform(glm::lookAt(_eyepoint, _lookAt, _up))
    {
    }

    void Render(const Scene &scene);

    //    s32 Intersection(const Scene &world, Ray ray, IntersectData &data, s32 check_obj);

    [[nodiscard]] const Frame &GetFrame() const { return _frame; }
    [[nodiscard]] glm::mat4 GetCameraTransform() const { return _camera_transform; }

  private:
    glm::vec4 CalculateLight(
        const Scene &scene, IntersectData &id, const Shader &i_model, std::vector<Light> lights, s32 obj, u32 depth);

    bool inShadow(const Scene &world, IntersectData &id, Light light, s32 obj);

    void tone_rep(TONE_TYPE which_type);
};
