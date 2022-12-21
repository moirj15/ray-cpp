#include "scene_importer.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../file_io.hpp"
#include "tiny_obj_loader.h"
#include "../geometry/shader.hpp"

#include <cassert>
#include <cstdio>
#include <vector>

Importer::Importer(const std::string &path) : m_filepath(path)
{
}

Scene Importer::Import()
{
    tinyobj::ObjReader reader;
    assert(reader.ParseFromFile(m_filepath));

    const tinyobj::attrib_t             &attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t> &shapes = reader.GetShapes();

    std::vector<u32>       indices;
    std::vector<glm::vec3> vertices;

    for (const auto &shape : shapes) {
        for (const auto &index : shape.mesh.indices) {
            indices.emplace_back(index.vertex_index);
        }
    }
    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        vertices.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }

    Scene scene;

    auto *mesh = new Mesh(std::move(vertices), std::move(indices), new FixedColor({1.0, 1.0, 0.0}));
    scene.AddObject(mesh);

    return scene;
}

#if 0
// namespace parse_utils
//{
//
// f32 ReadFloat(std::string_view file)
//{
//     std::string num;
//     for (s32 i = 0; i < file.size() && !iswspace(file[i]); i++) {
//     }
// }
//
// u32 ReadInt(std::string_view file)
//{
//     std::string num;
//     for (s32 i = 0; i < file.size() && !iswspace(file[i]); i++) {
//     }
// }
//
// }

#define ASSERT(X, MESSAGE) assert((X) && MESSAGE)

Importer::Importer(const std::string &path) : m_file(file_io::ReadFile(path))
{
}

Scene Importer::Import()
{
    using std::string_view;
    Scene scene;
    std::vector<glm::vec3> vertices;
    std::vector<u32> vert_indices;
    std::vector<u32> tex_indices;
    while (m_index < m_file.size()) {
        string_view token = ConsumeToken();
        if (token == "#") {
            SkipLine();
            ASSERT(!iswspace(m_file[m_index]), "Should be on line after comment");
        } else if (token == "v") {
            f64 x = std::stod(std::string(ConsumeToken()));
            f64 y = std::stod(std::string(ConsumeToken()));
            f64 z = std::stod(std::string(ConsumeToken()));
            ASSERT(!isdigit(m_file[m_index]), "Found an obj file that actually has the w component for vertex pos");
            vertices.emplace_back(x, y, z);
        } else if (token == "vt") {
            f64 v = std::stod(std::string(ConsumeToken()));
            f64 t = std::stod(std::string(ConsumeToken()));
            ASSERT(!isdigit(m_file[m_index]), "Found an obj file that actually has the w component for tex coords");
        } else if (token == "vn") {
            // TODO: Unused in raytracer, implement for rasterizer
            SkipLine();
        } else if (token == "vp") {
            SkipLine();
        } else if (token == "f") {
            string_view chunk = ConsumeToken();
            if (size_t slash_index = chunk.find_last_of('/'); slash_index != string_view::npos) {
                // TODO: Ignore the normal indices for now
                vert_indices.emplace_back(std::stoi(std::string(chunk.substr(0, slash_index - 1))) - 1);
                tex_indices.emplace_back(std::stoi(std::string(chunk.substr(slash_index + 1))) - 1);
            } else {
                vert_indices.emplace_back(std::stoi(std::string(chunk)) - 1);
            }
        } else if (token == "l") {
            // TODO: do lines some day
            SkipLine();
        } else {
            // TODO: skipping material files and groups for now
            SkipLine();
        }
    }
    // TODO: I don't like coupling the shader to the object, need a better solution
    scene.AddObject(new Mesh(vertices, vert_indices, new FixedColor({1.0, 1.0, 1.0})));
    return scene;
}

std::string_view Importer::ConsumeToken()
{
    const u32 start = m_index;
    u32 end = m_index;
    for (; end < m_file.size() && !iswspace(m_file[end]); end++) {
    }
    m_index = end + 1;
    if (iscntrl(m_file[m_index])) {
        m_index++;
    }
    assert(!iswspace(m_file[m_index]));
    return {m_file.begin() + start, m_file.begin() + end};
}

void Importer::SkipLine()
{
    m_index = m_file.find("\n", m_index) + 1;
}
#endif
