#pragma once

namespace sv
{

struct RenderContext;
class ResourceManager;

class Renderer
{
    RenderContext   &m_ctx;
    ResourceManager &m_resource_manager;

public:
    explicit Renderer(RenderContext &ctx, ResourceManager &rm);
    void DrawScene();
};

}
