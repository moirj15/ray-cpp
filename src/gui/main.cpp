#include <QApplication>
#include <QThread>
#include <QWidget>
#include <d3d11_3.h>
#include <d3d11shader.h>
#include <wrl/client.h>
#include "../utils.hpp"
#define Check(x) assert(SUCCEEDED(x))
using Microsoft::WRL::ComPtr;
class SceneViewer : public QWidget
{
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
    explicit SceneViewer(QWidget *parent) : QWidget(parent)
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
protected:
    void resizeEvent(QResizeEvent *event) override {

    }
};

class Application : public QApplication {
    QThread *m_backend_thread;
public:
    Application(int argc, char **argv) : QApplication(argc, argv), m_backend_thread(new QThread(this)) {}
};

int main(int argc, char **argv)
{
    Application app(argc, argv);
    SceneViewer scene_viewer(nullptr);
    scene_viewer.resize(1920, 1080);
    scene_viewer.show();
    return QApplication::exec();
}
