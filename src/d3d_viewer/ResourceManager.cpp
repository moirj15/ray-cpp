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
    glm::mat4 view_projection;
};

struct ModelConstants {
    glm::mat4 model_transform;
};

ResourceManager::ResourceManager(RenderContext &ctx) : m_ctx(ctx), m_projection(glm::infinitePerspective(90.0f, 16.0f / 9.0f, 0.001f))
{
    const Constants constants = {.view_projection = m_projection * m_camera};
    m_static_constants        = AllocateBuffer(static_cast<const void *>(&constants), sizeof(Constants), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER);
}

void ResourceManager::AddMesh(const std::vector<glm::vec3> &vertices, const std::vector<u32> &indices, const glm::mat4 &transform)
{
    m_vertex_buffers.emplace_back(Buffer{
        .buffer         = AllocateBuffer(vertices.data(), vertices.size() * sizeof(glm::vec3), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER),
        .size           = static_cast<u32>(vertices.size() * sizeof(glm::vec3)),
        .offsets        = 0,
        .element_stride = sizeof(glm::vec3),
    });
    m_index_buffers.emplace_back(IndexBuffer{
        .buffer = AllocateBuffer(indices.data(), indices.size() * sizeof(u32), D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER),
        .byte_size = static_cast<u32>(indices.size() * sizeof(u32)),
        .count = static_cast<u32>(indices.size()),
    });
    ModelConstants model_constants = {.model_transform = transform};
    m_constant_buffers.emplace_back(
        AllocateBuffer(static_cast<const void *>(&model_constants), sizeof(model_constants), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER));
}

void ResourceManager::ClearMeshes()
{
    m_vertex_buffers.clear();
    m_index_buffers.clear();
    m_constant_buffers.clear();
}

void ResourceManager::UpdateCameraMatrix(const glm::mat4 &camera)
{
    m_camera                  = camera;
    const Constants constants = {.view_projection = m_projection * m_camera};
    m_ctx.m_context->UpdateSubresource(m_static_constants.Get(), 0, nullptr, static_cast<const void *>(&constants), sizeof(constants), 0);
}

ID3D11Buffer *ResourceManager::AllocateBuffer(const void *data, const u32 size, D3D11_USAGE usage, D3D11_BIND_FLAG bind_flag)
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
    return buffer;
}
} // namespace sv
