#pragma once
#include <d3dcompiler.h>
class Scene;
class Camera;
#include "../utils.hpp"

#include <Windows.h>
#include <d3d11_3.h>
#include <d3d11shader.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <vector>
#include <wrl/client.h>
#include "ResourceManager.hpp"
#include "Renderer.hpp"

struct Window {
    HWND handle;
    s32  width;
    s32  height;
};
#define Check(x) assert(SUCCEEDED(x))

using Microsoft::WRL::ComPtr;
class Scene;
namespace sv
{

struct RenderContext {
    ComPtr<ID3D11Device3>         m_device;
    ComPtr<ID3D11DeviceContext3>  m_context;
    ComPtr<ID3D11RasterizerState> m_rasterizer_state;

    ComPtr<IDXGISwapChain>         m_swapchain;
    ComPtr<ID3D11Texture2D>        m_backbuffer;
    ComPtr<ID3D11RenderTargetView> m_backbuffer_render_target_view;
    ComPtr<ID3D11Texture2D>        m_depth_stencil_buffer;
    ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;

    RenderContext(HWND window_handle, u32 window_width, u32 window_height);
};

struct Mesh {
    glm::vec3 pos;
};

class SceneViewer
{
    const Scene  &m_scene;
    Camera &m_camera;

    RenderContext   m_ctx;
    ResourceManager m_resource_manager;
    Renderer        m_renderer;

    struct ObjectConstants {
        glm::mat4 mvp;
        glm::vec3 color;
    };

    D3D11_VIEWPORT m_viewport;

public:
    explicit SceneViewer(const Window &window, const Scene &scene, Camera &camera);

    void SetupScene(const Scene &scene);

    void MoveCamera(const glm::vec3 &target);
    void RotateCamera(const glm::vec2 &rotation);

    void Tick();
    void SwapBuffers();
};

} // namespace sv
