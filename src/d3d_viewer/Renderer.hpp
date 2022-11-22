#pragma once

namespace sv
{

struct RenderContext;
class ResourceManager;

class Renderer
{
    RenderContext         &m_ctx;
    const ResourceManager &m_resource_manager;

public:
    explicit Renderer(RenderContext &ctx, const ResourceManager &rm);
    void DrawScene();
};

} // namespace sv
