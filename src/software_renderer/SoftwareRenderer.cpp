#include "SoftwareRenderer.hpp"

#include "../geometry/scene.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "../Camera.h"

#include <glm/gtx/transform.hpp>

namespace ra
{

FrameBuffer FrameBuffer::Create(const u32 width, const u32 height)
{
    return {
        .width  = width,
        .height = height,
        .buffer = std::vector<u32>(width * height),
    };
}

SoftwareRenderer::SoftwareRenderer(u32 frame_buffer_width, u32 frame_buffer_height, Camera &camera) :
    m_frame_buffer(frame_buffer_width, frame_buffer_height),
    m_camera(camera)
{
}

void SoftwareRenderer::SetupScene(const Scene &scene) { m_scene = scene; }

void SoftwareRenderer::Draw()
{
    // TODO: add bvh traversal
    // TODO: also handle non-mesh objects
    //    for (const auto &object : m_scene->GetGeometries()) {
    //        if (object->type == Geometry::Type::Mesh) {
    //            auto *mesh = static_cast<Mesh *>(object.get());
    //        }
    //    }
}

Mesh SoftwareRenderer::TransformMesh(const Mesh &mesh, const glm::mat4 &transform)
{
    const glm::mat4 projection = glm::infinitePerspective(90.0f, 16.0f / 9.0f, 0.001f);
    const glm::mat4 view       = m_camera.CalculateMatrix();

    const glm::mat4 mvp          = projection * view * transform;
    const glm::mat4 screen_space = {
        m_frame_buffer.width, 0.0f, 0.0f, m_frame_buffer.width,   //
        1.0f, m_frame_buffer.height, 0.0f, m_frame_buffer.height, //
        0.0f, 0.0f, 1.0f, 0.0f,                                   //
        0.0f, 0.0f, 0.0f, 1.0f                                    //
    };
    std::vector<glm::vec3> transformed_vertices;
    for (const auto &vertex : mesh.GetVertices()) {
        glm::vec4 transformed_vert = mvp * glm::vec4(vertex, 1.0f);
        // TODO: May need to save off the vertex normal, uv, etc at this point for later interpolation during shading
        transformed_vert = screen_space * transformed_vert;
        transformed_vertices.emplace_back(transformed_vert.x / transformed_vert.w,
            transformed_vert.y / transformed_vert.w, transformed_vert.z / transformed_vert.w);
    }

    std::vector<u32> indices = mesh.GetIndices();
    // TODO: need a better way to move this data around
    // TODO: don't bind the shader to the object this way
    return Mesh(std::move(transformed_vertices), std::move(indices));
}

std::vector<glm::vec3> SoftwareRenderer::ClipTriangles(const Mesh &mesh) { return {}; }

} // namespace ra
