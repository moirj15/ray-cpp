#pragma once

#include "Common.hpp"

namespace sv
{

struct RenderContext;
class ResourceManager;
class Shaders;

class Renderer
{
    RenderContext         &m_ctx;
    const ResourceManager &m_resource_manager;
    const Shaders         &m_shaders;

    ComPtr<ID3D11RasterizerState> m_state;

public:
    explicit Renderer(RenderContext &ctx, const ResourceManager &rm, const Shaders &shaders);
    void DrawScene();
};

} // namespace sv
