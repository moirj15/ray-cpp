#pragma once
#include "Common.hpp"
#include "../utils.hpp"

#include <unordered_map>

class ID3D11Buffer;

namespace sv
{

struct RenderContext;

struct Buffer {
    ComPtr<ID3D11Buffer> buffer;
    size_t               size;
    void                 Clear();
};

MAKE_HANDLE(ResourceHandle);

class ResourceManager
{
    RenderContext *m_ctx;

    Buffer m_vertices;
    Buffer m_indices;

    struct MeshSection {
        ResourceHandle handle;
        u32            start;
        u32            end;
    };

    std::unordered_map<ResourceHandle, MeshSection> m_sections;

public:
    explicit ResourceManager(RenderContext *ctx);
    void AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices);

    const ID3D11Buffer *GetVertexBuffer() const
    {
        return m_vertices.buffer.Get();
    }

    const ID3D11Buffer *GetIndexBufferBuffer() const
    {
        return m_indices.buffer.Get();
    }

    void Clear();

private:
    Buffer AllocateBuffer(const void *data, const size_t size, D3D11_USAGE usage, D3D11_BIND_FLAG bind_flag);
    void   PushBack(Buffer &buffer, const void *data, const size_t size, D3D11_BIND_FLAG bind_flag);
};

} // namespace sv