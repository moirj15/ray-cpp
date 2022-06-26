#pragma once
#include <d3dcompiler.h>
class Scene;
class Camera;
#include "utils.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <Windows.h>
#include <d3d11_3.h>
#include <d3d11shader.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <wrl/client.h>

struct Window {
    SDL_Window *sdl_window;
    s32 width;
    s32 height;
};
#define Check(x) assert(SUCCEEDED(x))

using Microsoft::WRL::ComPtr;
class SceneViewer
{
    const Scene &m_scene;
    const Camera &m_camera;

    struct ObjectConstants {
        glm::mat4 mvp;
        glm::vec3 color;
    };

    std::vector<ObjectConstants> m_constants;
    std::vector<glm::vec3> m_vertices;

    ComPtr<ID3D11Device3> m_device;
    ComPtr<ID3D11DeviceContext3> m_context;
    ComPtr<ID3D11RasterizerState> m_rasterizer_state;

    ComPtr<IDXGISwapChain> m_swapchain;
    ComPtr<ID3D11Texture2D> m_backbuffer;
    ComPtr<ID3D11RenderTargetView> m_backbuffer_render_target_view;
    ComPtr<ID3D11Texture2D> m_depth_stencil_buffer;
    ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;
    D3D11_VIEWPORT m_viewport;

  public:
    explicit SceneViewer(const Scene &scene, const Camera &camera);
    Window MakeWindow(const char *title, int32_t width, int32_t height);
};
