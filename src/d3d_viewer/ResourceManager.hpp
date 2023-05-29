#pragma once
#include "../utils.hpp"
#include "Common.hpp"

#include <unordered_map>

class Scene;

namespace sv
{

struct RenderContext;

struct VertexBuffer {
    ComPtr<ID3D11Buffer> buffer;
    size_t               size           = 0;
    u32                  offsets        = 0;
    u32                  element_stride = 0;

    void Clear();
};

struct IndexBuffer {
    ComPtr<ID3D11Buffer> buffer;
    size_t               byte_size = 0;
    size_t               count     = 0;
};

struct ConstantsBuffer {
    ComPtr<ID3D11Buffer> buffer;
    size_t               size  = 0;
    size_t               count = 0;
};

class ResourceManager
{
    RenderContext &m_ctx;

    VertexBuffer         m_vert_buf;
    IndexBuffer          m_index_buf;
    ConstantsBuffer      m_constants_buf;
    ComPtr<ID3D11Buffer> m_static_constants;

    // std::vector<Buffer>               m_vertex_buffers;
    // std::vector<IndexBuffer>          m_index_buffers;
    // std::vector<ComPtr<ID3D11Buffer>> m_constant_buffers;

    glm::mat4 m_projection;
    glm::mat4 m_camera;

public:
    explicit ResourceManager(RenderContext &ctx);
    void PopulateFromScene(const Scene &scene);
    void AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices, const glm::mat4 &transform = glm::mat4(1.0));

    void UpdateCameraMatrix(const glm::mat4 &camera);

    void ClearMeshes();

    const VertexBuffer &GetVertexBuffer() const { return m_vert_buf; }

    const IndexBuffer &GetIndexBuffer() const { return m_index_buf; }

    const ID3D11Buffer *GetConstantsBuffer() const { return m_constants_buf.buffer.Get(); }

    ID3D11Buffer *GetStaticConstants() const { return m_static_constants.Get(); }

private:
    ID3D11Buffer *AllocateBuffer(const void *data, const u32 size, D3D11_USAGE usage, D3D11_BIND_FLAG bind_flag);
};

} // namespace sv
