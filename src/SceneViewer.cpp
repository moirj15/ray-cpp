#include "SceneViewer.hpp"

SceneViewer::SceneViewer(const Scene &scene, const Camera &camera) : m_scene(scene), m_camera(camera)
{
    u32 createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL desiredLevel[] = {D3D_FEATURE_LEVEL_11_1};
    D3D_FEATURE_LEVEL featureLevel;
    // TODO: get latest dx11
    ID3D11Device *baseDevice;
    ID3D11DeviceContext *baseContext;
    Check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, desiredLevel, 1,
        D3D11_SDK_VERSION, &baseDevice, &featureLevel, &baseContext));
    assert(featureLevel == D3D_FEATURE_LEVEL_11_1);

    baseDevice->QueryInterface(__uuidof(ID3D11Device3), &m_device);
    baseContext->QueryInterface(__uuidof(ID3D11DeviceContext3), &m_context);

    // TODO: temporary rasterizer state, should create a manager for this, maybe create a handle type for this?
    // TODO: maybe an internal handle just for tracking this internaly?
    D3D11_RASTERIZER_DESC rasterizerDesc = {
        .FillMode = D3D11_FILL_SOLID,
        .CullMode = D3D11_CULL_NONE,
        .FrontCounterClockwise = true,
    };
    m_viewport = {
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width = 0.0f,
        .Height = 0.0f,
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };

    m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizer_state);
}

Window SceneViewer::MakeWindow(const char *title, int32_t width, int32_t height)
{
    SDL_Window *window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    assert(window != nullptr);

    // Don't resize window so it doesn't mess with tiling window managers
    SDL_SetWindowResizable(window, SDL_FALSE);

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;

    // clang-format off
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {
            .BufferDesc = {
                .Width = (uint32_t)width,
                .Height = (uint32_t)height,
                .RefreshRate = {
                    .Numerator = 60,
                    .Denominator = 1,
                },
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
                .Scaling = DXGI_MODE_SCALING_UNSPECIFIED,
            },
        // Multi sampling would be initialized here
        .SampleDesc = {
            .Count = 1,
            .Quality = 0,
        },
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 1,
        .OutputWindow = hwnd,
        .Windowed = true,
        .SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
        .Flags = 0,
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
			.Width = (uint32_t)width,
			.Height = (uint32_t)height,
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

    // TODO: be careful about setting this to something else down the road. Currently not checking for changes
    m_viewport = {
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width = (float)width,
        .Height = (float)height,
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };

    m_context->RSSetViewports(1, &m_viewport);

    return {
        .sdl_window = window,
        .width = width,
        .height = height,
    };
}
