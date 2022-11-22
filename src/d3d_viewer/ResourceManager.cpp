#include "ResourceManager.hpp"

#include "SceneViewer.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace sv
{

void Buffer::Clear()
{
    buffer.Reset();
    size = 0;
}

struct Constants {
    glm::mat4 projection_view;
};

ResourceManager::ResourceManager(RenderContext &ctx) :
    m_ctx(ctx),
    m_camera_buf(AllocateBuffer(nullptr, sizeof(Constants), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER))
{
}

void ResourceManager::AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices)
{
    PushBack(m_vertices, static_cast<const void *>(vertices.data()), vertices.size() * sizeof(glm::vec3), D3D11_BIND_VERTEX_BUFFER);
    PushBack(m_indices, static_cast<const void *>(indices.data()), indices.size() * sizeof(u32), D3D11_BIND_INDEX_BUFFER);
}

void ResourceManager::ClearMeshes()
{
}

void ResourceManager::UpdateCameraMatrix(const glm::mat4 &camera)
{
    const glm::mat4 PROJECTION = glm::infinitePerspective(90.0, 16.0 / 9.0, 0.001);

    Constants constants = {
        .projection_view = PROJECTION * camera,
    };
    m_ctx.m_context->UpdateSubresource(m_camera_buf.buffer.Get(), 0, nullptr, static_cast<const void *>(&constants), sizeof(Constants), 0);
}

Buffer ResourceManager::AllocateBuffer(const void *data, const size_t size, D3D11_USAGE usage, D3D11_BIND_FLAG bind_flag)
{
    ID3D11Buffer     *buffer      = nullptr;
    D3D11_BUFFER_DESC buffer_desc = {
        .ByteWidth      = static_cast<UINT>(size),
        .Usage          = usage,
        .BindFlags      = static_cast<UINT>(bind_flag),
        .CPUAccessFlags = 0,
        .MiscFlags      = 0,
    };

    if (data) {
        D3D11_SUBRESOURCE_DATA buffer_data = {
            .pSysMem          = data,
            .SysMemPitch      = 0,
            .SysMemSlicePitch = 0,
        };
        Check(m_ctx.m_device->CreateBuffer(&buffer_desc, &buffer_data, &buffer));
    } else {
        Check(m_ctx.m_device->CreateBuffer(&buffer_desc, nullptr, &buffer));
    }
    return {.buffer = buffer, .size = size};
}

void ResourceManager::PushBack(Buffer &buffer, const void *data, const size_t size, D3D11_BIND_FLAG bind_flag)
{
    if (buffer.size > 0) {
        Buffer temp = AllocateBuffer(nullptr, size, D3D11_USAGE_DEFAULT, bind_flag);
        // TODO: this might not work. Might have to use CopySubresourceRegion()
        m_ctx.m_context->CopyResource(buffer.buffer.Get(), temp.buffer.Get());
        m_ctx.m_context->UpdateSubresource(temp.buffer.Get(), 0, nullptr, data, size, 0);
    } else {
        buffer = AllocateBuffer(data, size, D3D11_USAGE_DEFAULT, bind_flag);
    }
}
} // namespace sv
