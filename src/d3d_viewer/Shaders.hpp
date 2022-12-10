#pragma once

#include "Common.hpp"

#include <string>

namespace sv
{

struct RenderContext;

struct Shader {
    ComPtr<ID3D11VertexShader> vs;
    ComPtr<ID3D11PixelShader>  ps;
    ComPtr<ID3D11InputLayout>  layout; // TODO: move this to the shader
};

class Shaders
{
    RenderContext &m_ctx;
    Shader         m_flat_shader;

public:
    explicit Shaders(RenderContext &ctx);
    const Shader &GetFlatShader() const
    {
        return m_flat_shader;
    }

private:
    Shader    CreateShader(const std::string &filename);
    ID3DBlob *CompileShader(const std::string &filename, const char *entry_point, const char *shader_model);
};

} // namespace sv
