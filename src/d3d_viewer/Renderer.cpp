#include "Renderer.hpp"

#include "SceneViewer.hpp"
#include "Shaders.hpp"

namespace sv
{

Renderer::Renderer(RenderContext &ctx, const ResourceManager &rm, const Shaders &shaders) : m_ctx(ctx), m_resource_manager(rm), m_shaders(shaders)
{
    const Shader &flat = m_shaders.GetFlatShader();
    m_ctx.m_context->VSSetShader(flat.vs.Get(), nullptr, 0);
    m_ctx.m_context->PSSetShader(flat.ps.Get(), nullptr, 0);

    D3D11_RASTERIZER_DESC state_desc = {
        .FillMode = D3D11_FILL_SOLID,
        .CullMode = D3D11_CULL_NONE,
        .FrontCounterClockwise = true,
    };

    Check(m_ctx.m_device->CreateRasterizerState(&state_desc, &m_state));
}

void Renderer::DrawScene()
{
    m_ctx.m_context->IASetVertexBuffers(0, 1, &m_resource_manager);
}

} // namespace sv
