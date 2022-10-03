#pragma once

#include "../geometry/shader.hpp"
#include "../utils.hpp"

#define WIDTH  600
#define HEIGHT 400

#define PIC_SIZE (WIDTH * HEIGHT)

enum TONE_TYPE { WARD, REINHARD };

class Scene;
class Shader;
struct IntersectData;
struct Camera;
class Frame;

class Tracer
{
    const Scene &m_scene;
    const Camera &m_camera;
    Frame &m_frame;

public:
    Tracer(const Scene &scene, const Camera &camera, Frame &frame) : m_scene(scene), m_camera(camera), m_frame(frame) {}
    void RenderFrame();
};
