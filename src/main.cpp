#include "SDL2/SDL.h"
#include "bmpIO.h"
#include "camera.h"
#include "material.h"
#include "object.h"
#include "scene.h"
#include "shader.h"
#include "utils.h"

#include <D3dcompiler.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <cstdio>
#include <cstdlib>
#include <d3d11_3.h>
#include <d3d11shader.h>
#include <thread>
#include <vector>
#include <windows.h>
#include <wrl/client.h>

static const glm::vec4 ambient_mat(1.0, 1.0, 1.0, 1.0);
static const glm::vec4 diffuse_mat(1.0, 1.0, 1.0, 1.0);
static const glm::vec4 specular_mat(1.0, 1.0, 1.0, 1.0);

static const f32 film_plane_width = 16.0f;
static const f32 film_plane_height = 9.0f;
static const f32 ldmax = 1.0f;

struct Window {
    SDL_Window *sdl_window;
    s32 width;
    s32 height;
};

#define Check(x) assert(FAILED(x))

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
    explicit SceneViewer(const Scene &scene, const Camera &camera) : m_scene(scene), m_camera(camera)
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
        Check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, desiredLevel, 1,
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

        m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizer_state);
        for (const auto &object : m_scene.GetObjects()) {
            switch (object->type) {
            case Object::Type::Sphere:
                BuildSphere(reinterpret_cast<Sphere *>(object.get()));
                break;
            case Object::Type::Mesh:
                AddMesh(reinterpret_cast<Mesh *>(object.get()));
                break;
            }
        }
    }

    Window MakeWindow(const char *title, int32_t width, int32_t height)
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

  private:
    void BuildSphere(const Sphere *sphere) {}

    void AddMesh(const Mesh *mesh)
    {
        for (const auto &vertex : mesh->GetVertices()) {
            m_vertices.emplace_back(vertex);
        }
        // TODO: store matrix with objects?
        // m_constants.emplace_back(m_camera.GetCameraTransform() * )
    }
};

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("start\n");
    Scene scene;

    const Light light(
        glm::vec4(0.0f, 0.00f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    scene.AddLight(light);

    Phong sphere_illum1(scene, 0.0f, 0.5f, 0.5f, 70.0f, 0.0f, 0.8f, ambient_mat, diffuse_mat, specular_mat);
    Phong sphere_illum2(scene, 0.0f, 0.5f, 0.5f, 70.0f, 0.5f, 0.0f, ambient_mat, diffuse_mat, specular_mat);

    glm::vec4 color1(1.0, 0.0, 0.0, 0.0);
    glm::vec4 color2(1.0, 1.0, 0.0, 0.0);

    CheckerBoard floor_illum(scene, 0.5, 0.5, 0.5, 70.0, 0.0, 0.0, color1, color2);

    std::vector<glm::vec3> triangles;

    triangles.emplace_back(-1.0, 0.0, -1.0); // fl
    triangles.emplace_back(-1.0, 0.0, 1.0);  // bl
    triangles.emplace_back(1.0, 0.0, 1.0);   // br

    triangles.emplace_back(1.0, 0.0, 1.0);   // br
    triangles.emplace_back(1.0, 0.0, -1.0);  // tr
    triangles.emplace_back(-1.0, 0.0, -1.0); // fl

    for (auto &p : triangles) {
        auto t = glm::translate(glm::mat4(1.0), {0.0, -1.9, 0.0});
        p = glm::scale(t, {10, 5, 10}) * glm::vec4(p, 1.0);
    }

    auto plane_transform = glm::scale(glm::translate(glm::mat4(1.0), {0.0, -1.9, 0.0}), {10, 5, 10});

    scene.AddObject(new Mesh(triangles, &floor_illum), plane_transform);

    scene.AddObject(new Sphere(glm::vec4(-1.0f, 0.0f, 0.4f, 1.0f), 1.00f, &sphere_illum1));
    scene.AddObject(new Sphere(glm::vec4(0.3f, 0.3f, -0.3f, 1.0), 0.8, &sphere_illum2));

    const glm::vec3 camera_pos(0.0f, 0.0f, 1.0f);
    const glm::vec3 look_at(0.0f, 0.0f, -1.0f);
    const glm::vec3 up_vec(0.0f, 1.0f, 0.0f);
    Camera camera(camera_pos, look_at, up_vec, film_plane_width, film_plane_height, 1.0f);

    SceneViewer scene_viewer(scene, camera);
    Window window = scene_viewer.MakeWindow("preview", 640, 480);
    SDL_Event e;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&e) > 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    // Getting the gl-preview working
#if 0
    //    auto ray_thread = std::thread([&scene, &camera]() {
    scene.Transform(camera.GetCameraTransform());
    camera.Render(scene);

    const auto &frame = camera.GetFrame();
    bmp::Write("test.bmp", frame);
    printf("finished\n");
    //    });

    //    SDL_Event e;
    //    bool running = true;
    //    auto *device = focus::Device::Init(focus::RendererAPI::OpenGL);
    //    auto window = device->MakeWindow(1920, 1080);
    //    while (running) {
    //        while (SDL_PollEvent(&e) > 0) {
    //            if (e.type == SDL_QUIT) {
    //                running = false;
    //            }
    //        }
    //    }
    //
    //    ray_thread.join();
#endif

    return EXIT_SUCCESS;
}
