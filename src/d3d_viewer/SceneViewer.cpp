#include "SceneViewer.hpp"

#include "../Camera.h"
#include "../geometry/object.hpp"
#include "../geometry/scene.hpp"

namespace sv
{

RenderContext::RenderContext(HWND window_handle, u32 window_width, u32 window_height)
{
    u32 createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL desiredLevel[] = {D3D_FEATURE_LEVEL_11_1};
    D3D_FEATURE_LEVEL featureLevel;
    // TODO: get latest dx11
    ID3D11Device        *baseDevice;
    ID3D11DeviceContext *baseContext;
    Check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, desiredLevel, 1,
        D3D11_SDK_VERSION, &baseDevice, &featureLevel, &baseContext));
    assert(featureLevel == D3D_FEATURE_LEVEL_11_1);

    baseDevice->QueryInterface(__uuidof(ID3D11Device3), &m_device);
    baseContext->QueryInterface(__uuidof(ID3D11DeviceContext3), &m_context);

    // TODO: temporary rasterizer state, should create a manager for this, maybe create a handle type for this?
    // TODO: maybe an internal handle just for tracking this internaly?
    D3D11_RASTERIZER_DESC rasterizerDesc = {
        .FillMode              = D3D11_FILL_SOLID,
        .CullMode              = D3D11_CULL_NONE,
        .FrontCounterClockwise = true,
    };

    m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizer_state);

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {
        .BufferDesc =
            {
                .Width  = window_width,
                .Height = window_height,
                .RefreshRate =
                    {
                        .Numerator   = 60,
                        .Denominator = 1,
                    },
                .Format           = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
                .Scaling          = DXGI_MODE_SCALING_UNSPECIFIED,
            },
        // Multi sampling would be initialized here
        .SampleDesc =
            {
                .Count   = 1,
                .Quality = 0,
            },
        .BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount  = 1,
        .OutputWindow = window_handle,
        .Windowed     = true,
        .SwapEffect   = DXGI_SWAP_EFFECT_DISCARD,
        .Flags        = 0,
    };
    // clang-format on

    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    Check(m_device->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice));
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    Check(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), &adapter));
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    Check(adapter->GetParent(__uuidof(IDXGIFactory), &factory));

    Check(factory->CreateSwapChain(m_device.Get(), &swapChainDesc, &m_swapchain));

    m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &m_backbuffer);
    m_device->CreateRenderTargetView(m_backbuffer.Get(), 0, &m_backbuffer_render_target_view);

    // clang-format off
    D3D11_TEXTURE2D_DESC depthStencilDesc = {
        .Width = window_width,
        .Height = window_height,
        .MipLevels = 1,
        .ArraySize = 1,
        .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
        // Multi sampling here
        .SampleDesc = {
            .Count = 1,
            .Quality = 0,
        },
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_DEPTH_STENCIL,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
    };
    // clang-format on

    Check(m_device->CreateTexture2D(&depthStencilDesc, 0, &m_depth_stencil_buffer));
    Check(m_device->CreateDepthStencilView(m_depth_stencil_buffer.Get(), 0, &m_depth_stencil_view));
    m_context->OMSetRenderTargets(1, m_backbuffer_render_target_view.GetAddressOf(), m_depth_stencil_view.Get());
}

SceneViewer::SceneViewer(const Window &window, const Scene &scene, Camera &camera) :
    m_scene(scene),
    m_camera(camera),
    m_ctx(window.handle, window.width, window.height),
    m_resource_manager(m_ctx),
    m_shaders(m_ctx),
    m_renderer(m_ctx, m_resource_manager, m_shaders)
{
    // TODO: be careful about setting this to something else down the road. Currently not checking for changes
    m_viewport = {
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width    = (float)window.width,
        .Height   = (float)window.height,
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };

    m_ctx.m_context->RSSetViewports(1, &m_viewport);

    // for (const auto &[handle, geometry] : m_scene.GetGeometries()) {
    //     if (geometry->type == Geometry::Type::Mesh) {
    //         auto *mesh = reinterpret_cast<::Mesh *>(geometry.get());
    //         m_resource_manager.AddMesh(mesh->GetVertices(), mesh->GetIndices());
    //     }
    // }
    pipelines::Init(m_ctx);
}

void SceneViewer::SetupScene(const Scene &scene) { m_resource_manager.PopulateFromScene(scene); }

void SceneViewer::MoveCamera(const glm::vec3 &target) { m_camera.Move(target); }

void SceneViewer::RotateCamera(const glm::vec2 &rotation) { m_camera.Rotate(rotation); }

void SceneViewer::Tick()
{
    if (m_camera.IsDirty()) {
        m_resource_manager.UpdateCameraMatrix(m_camera.CalculateMatrix());
    }
    m_renderer.DrawScene();
}

void SceneViewer::SwapBuffers()
{
    m_ctx.m_swapchain->Present(1, 0);
    m_ctx.m_context->ClearDepthStencilView(
        m_ctx.m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
}

} // namespace sv
