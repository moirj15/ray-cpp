#pragma once

#include "../utils.hpp"

#include <vector>
#include "../geometry/object.hpp"

class Scene;
class Camera;

namespace ra
{

struct FrameBuffer
{
    const u32              width = 0;
    const u32              height = 0;
    const std::vector<u32> buffer;
    static FrameBuffer Create(u32 width, u32 height);
};

class SoftwareRenderer
{
    FrameBuffer m_frame_buffer;
    Scene      *m_scene = nullptr;
    Camera     &m_camera;

public:
    explicit SoftwareRenderer(u32 frame_buffer_width, u32 frame_buffer_height, Camera &camera);
    void               SetupScene(const Scene &scene);
    void               Draw();
    const FrameBuffer &GetFrameBuffer() const
    {
        return m_frame_buffer;
    }

private:
    Mesh TransformMesh(const Mesh &mesh, const glm::mat4 &transform);
    std::vector<glm::vec3> ClipTriangles(const Mesh &mesh);
};

} // namespace ra