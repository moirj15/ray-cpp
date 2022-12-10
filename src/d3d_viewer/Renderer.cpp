#include "Renderer.hpp"

#include "SceneViewer.hpp"
#include "Shaders.hpp"

namespace sv
{

Renderer::Renderer(RenderContext &ctx, const ResourceManager &rm, const Shaders &shaders) :
    m_ctx(ctx),
    m_resource_manager(rm),
    m_shaders(shaders)
{
    const Shader &flat = m_shaders.GetFlatShader();
    m_ctx.m_context->VSSetShader(flat.vs.Get(), nullptr, 0);
    m_ctx.m_context->PSSetShader(flat.ps.Get(), nullptr, 0);
    ID3D11Buffer *constants = m_resource_manager.GetStaticConstants();
    m_ctx.m_context->VSSetConstantBuffers(0, 1, &constants);

    D3D11_RASTERIZER_DESC state_desc = {
        .FillMode              = D3D11_FILL_SOLID,
        .CullMode              = D3D11_CULL_NONE,
        .FrontCounterClockwise = true,
    };

    Check(m_ctx.m_device->CreateRasterizerState(&state_desc, &m_state));
    m_ctx.m_context->RSSetState(m_state.Get());
    m_ctx.m_context->OMSetRenderTargets(1, m_ctx.m_backbuffer_render_target_view.GetAddressOf(), m_ctx.m_depth_stencil_view.Get());
}

void Renderer::DrawScene()
{
    m_ctx.m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_ctx.m_context->IASetInputLayout(m_shaders.GetFlatShader().layout.Get());
    const auto &vertex_buffers   = m_resource_manager.GetVertexBuffers();
    const auto &index_buffers    = m_resource_manager.GetIndexBuffers();
    const auto &constant_buffers = m_resource_manager.GetConstantBuffers();
    for (size_t i = 0; i < vertex_buffers.size(); i++) {
        m_ctx.m_context->IASetVertexBuffers(0, 1, vertex_buffers[i].buffer.GetAddressOf(), &vertex_buffers[i].element_stride, &vertex_buffers[i].offsets);
        m_ctx.m_context->IASetIndexBuffer(index_buffers[i].buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        m_ctx.m_context->VSSetConstantBuffers(1, 1, constant_buffers[i].GetAddressOf());
        m_ctx.m_context->DrawIndexed(index_buffers[i].count, 0, 0);
    }
}

} // namespace sv
