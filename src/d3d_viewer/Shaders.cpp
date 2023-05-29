#include "Shaders.hpp"

#include "../file_io.hpp"
#include "../utils.hpp"
#include "SceneViewer.hpp"

#include <d3dcompiler.h>
#include <vector>

namespace sv
{

Shaders::Shaders(RenderContext &ctx) : m_ctx(ctx), m_flat_shader(CreateShader("shaders/flat.hlsl"))
{
    assert(m_flat_shader.vs.Get() != nullptr); // sanity check
}

Shader Shaders::CreateShader(const std::string &filename)
{
    ComPtr<ID3DBlob>    vs_binary = CompileShader(filename, "VSMain", "vs_5_0");
    ID3D11VertexShader *vs        = nullptr;
    Check(m_ctx.m_device->CreateVertexShader(vs_binary->GetBufferPointer(), vs_binary->GetBufferSize(), nullptr, &vs));

    ID3D11InputLayout       *input_layout = nullptr;
    D3D11_INPUT_ELEMENT_DESC input_desc[] = {
        {
            .SemanticName         = "POSITION",
            .SemanticIndex        = 0,
            .Format               = DXGI_FORMAT_R32G32B32_FLOAT,
            .InputSlot            = 0,
            .AlignedByteOffset    = 0,
            .InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA,
            .InstanceDataStepRate = 0,
        },
    };
    Check(m_ctx.m_device->CreateInputLayout(input_desc, 1, vs_binary->GetBufferPointer(), vs_binary->GetBufferSize(), &input_layout));

    ComPtr<ID3DBlob>   ps_binary = CompileShader(filename, "PSMain", "ps_5_0");
    ID3D11PixelShader *ps        = nullptr;
    Check(m_ctx.m_device->CreatePixelShader(ps_binary->GetBufferPointer(), ps_binary->GetBufferSize(), nullptr, &ps));

    return {.vs = vs, .ps = ps, .layout = input_layout};
}

ID3DBlob *Shaders::CompileShader(const std::string &filename, const char *entry_point, const char *shader_model)
{
    const std::string source = file_io::ReadFile(filename);

    ID3DBlob        *binary = nullptr;
    ComPtr<ID3DBlob> errors;

    const u32 flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

    if (FAILED(D3DCompile(source.data(), source.size(), nullptr, nullptr, nullptr, entry_point, shader_model, flags, 0, &binary, &errors))) {
        printf("Shader Compile Error: %s\n", static_cast<char *>(errors->GetBufferPointer()));
        assert(0);
    }
    return binary;
}

static ID3DBlob *CompileShader(const std::string &filename, const char *entry_point, const char *shader_model)
{
    const std::string source = file_io::ReadFile(filename);

    ID3DBlob        *binary = nullptr;
    ComPtr<ID3DBlob> errors;

    const u32 flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

    if (FAILED(D3DCompile(source.data(), source.size(), nullptr, nullptr, nullptr, entry_point, shader_model, flags, 0, &binary, &errors))) {
        printf("Shader Compile Error: %s\n", static_cast<char *>(errors->GetBufferPointer()));
        assert(0);
    }
    return binary;
}

struct VSPair {
    ID3D11VertexShader *vs;
    ID3D11InputLayout *input_layout;
};

// TODO: opening a file twice, don't do that
static VSPair CreateVertShaderAndInputLayout(const RenderContext &context, const std::string &filename, D3D11_INPUT_ELEMENT_DESC input_desc[])
{
    ComPtr<ID3DBlob>    vs_binary = CompileShader(filename, "VSMain", "vs_5_0");
    ID3D11VertexShader *vs        = nullptr;
    Check(context.m_device->CreateVertexShader(vs_binary->GetBufferPointer(), vs_binary->GetBufferSize(), nullptr, &vs));

    ID3D11InputLayout       *input_layout = nullptr;
    Check(context.m_device->CreateInputLayout(input_desc, 1, vs_binary->GetBufferPointer(), vs_binary->GetBufferSize(), &input_layout));
    return {vs, input_layout};
}

static ID3D11PixelShader *CompilePixelShader(const RenderContext &context, const std::string &filename)
{
    ComPtr<ID3DBlob>    ps_binary = CompileShader(filename, "PSMain", "ps_5_0");
    ID3D11PixelShader *ps        = nullptr;
    Check(context.m_device->CreatePixelShader(ps_binary->GetBufferPointer(), ps_binary->GetBufferSize(), nullptr, &ps));

    return ps;
}

namespace pipelines {

static RenderPipeline m_flat;

void Init(const RenderContext &context) {

    D3D11_INPUT_ELEMENT_DESC input_desc[] = {
        {
            .SemanticName         = "POSITION",
            .SemanticIndex        = 0,
            .Format               = DXGI_FORMAT_R32G32B32_FLOAT,
            .InputSlot            = 0,
            .AlignedByteOffset    = 0,
            .InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA,
            .InstanceDataStepRate = 0,
        },
    };
    auto [vs, input_layout] = CreateVertShaderAndInputLayout(context, "shaders/flat.hlsl", input_desc);

    D3D11_RASTERIZER_DESC rasterizerDesc = {
        .FillMode              = D3D11_FILL_SOLID,
        .CullMode              = D3D11_CULL_NONE,
        .FrontCounterClockwise = true,
    };

    ID3D11RasterizerState *rasterizer_state = nullptr;
    context.m_device->CreateRasterizerState(&rasterizerDesc, &rasterizer_state);
    m_flat = {
        .primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        .vertex_layout = input_layout,
        .vs = vs,
        .ps = CompilePixelShader(context, "shaders/flat.hlsl"),
        .rasterizer_state = rasterizer_state,
    };
}

RenderPipeline GetFlatPipeline() {
    return m_flat;
}

void Shutdown() {

}

}

} // namespace sv
