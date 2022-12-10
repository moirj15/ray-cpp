#pragma once
#include "../utils.hpp"
#include "Common.hpp"

#include <unordered_map>

namespace sv
{

struct RenderContext;

struct Buffer {
    ComPtr<ID3D11Buffer> buffer;
    u32                  size           = 0;
    u32                  offsets        = 0;
    u32                  element_stride = 0;

    void Clear();
};

struct IndexBuffer {
    ComPtr<ID3D11Buffer> buffer;
    u32                  byte_size = 0;
    u32                  count     = 0;
};

class ResourceManager
{
    RenderContext &m_ctx;

    std::vector<Buffer>               m_vertex_buffers;
    std::vector<IndexBuffer>          m_index_buffers;
    std::vector<ComPtr<ID3D11Buffer>> m_constant_buffers;
    ComPtr<ID3D11Buffer>              m_static_constants;

    glm::mat4 m_projection;
    glm::mat4 m_camera;

public:
    explicit ResourceManager(RenderContext &ctx);
    void AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices, const glm::mat4 &transform = glm::mat4(1.0));

    void UpdateCameraMatrix(const glm::mat4 &camera);

    void ClearMeshes();

    const std::vector<Buffer> &GetVertexBuffers() const
    {
        return m_vertex_buffers;
    }

    const std::vector<IndexBuffer> &GetIndexBuffers() const
    {
        return m_index_buffers;
    }

    const std::vector<ComPtr<ID3D11Buffer>> &GetConstantBuffers() const
    {
        return m_constant_buffers;
    }

    ID3D11Buffer *GetStaticConstants() const
    {
        return m_static_constants.Get();
    }

private:
    ID3D11Buffer *AllocateBuffer(const void *data, const u32 size, D3D11_USAGE usage, D3D11_BIND_FLAG bind_flag);
};

} // namespace sv
