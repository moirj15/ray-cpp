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
    u32                  element_stride = 0;
    u32                  offsets        = 0;

    void Clear();
};

MAKE_HANDLE(ConstantHandle);

class ResourceManager
{
    RenderContext &m_ctx;

    Buffer m_vertices;
    Buffer m_indices;

    struct MeshSection {
        ConstantHandle handle;
        u32            vertex_start;
        u32            index_start;
    };

    std::vector<MeshSection>                   m_sections;
    std::unordered_map<ConstantHandle, Buffer> m_constant_buffs;

    Buffer m_camera_buf;

    ConstantHandle m_next_handle;

public:
    explicit ResourceManager(RenderContext &ctx);
    void AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices, const glm::mat4 &transform = glm::mat4(1.0));

    void UpdateCameraMatrix(const glm::mat4 &camera);

    void ClearMeshes();

    const ID3D11Buffer *GetVertexBuffer() const
    {
        return m_vertices.buffer.Get();
    }

    const ID3D11Buffer *GetIndexBuffer() const
    {
        return m_indices.buffer.Get();
    }

    const ID3D11Buffer *GetConstantBuffer() const
    {
        return m_camera_buf.buffer.Get();
    }

    const std::vector<MeshSection> &GetSections() const
    {
        return m_sections;
    }

    const Buffer &GetConstantBuffer(const ConstantHandle handle)
    {
        // Shouldn't produce a default Buffer object. Assuming the handle comes from an entry in the m_sections vector.
        return m_constant_buffs[handle];
    }

private:
    Buffer AllocateBuffer(const void *data, const u32 size, D3D11_USAGE usage, D3D11_BIND_FLAG bind_flag);
    void   PushBack(Buffer &buffer, const void *data, const size_t size, D3D11_BIND_FLAG bind_flag);
};

} // namespace sv
