#pragma once

#include "../utils.hpp"

#include <vector>
#include "../geometry/object.hpp"

class Scene;
class Camera;

namespace ra
{

class FrameBuffer
{
    std::vector<u32> m_buffer;
    u32              m_width;
    u32              m_height;

public:
    explicit FrameBuffer(u32 width, u32 height) : m_buffer(width * height), m_width(width), m_height(height)
    {
    }

    const u32 *GetBuffer() const
    {
        return m_buffer.data();
    }
    size_t GetBufferSize() const
    {
        return m_buffer.size();
    }
    u32 GetWidth() const
    {
        return m_width;
    }
    u32 GetHeight() const
    {
        return m_height;
    }
};

class SoftwareRenderer
{
    FrameBuffer m_frame_buffer;
    Scene      *m_scene = nullptr;
    Camera     &m_camera;

public:
    explicit SoftwareRenderer(u32 frame_buffer_width, u32 frame_buffer_height, Camera &camera);
    void               SetupScene(Scene *scene);
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