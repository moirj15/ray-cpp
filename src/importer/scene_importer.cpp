#include "scene_importer.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../file_io.hpp"
#include "../geometry/shader.hpp"
#include "tiny_obj_loader.h"

#include <cassert>
#include <cstdio>
#include <vector>

namespace imp
{

static Scene ImportObj(const std::string &filepath)
{
    tinyobj::ObjReader reader;
    assert(reader.ParseFromFile(filepath));

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

    auto *mesh = new Mesh(std::move(vertices), std::move(indices));
    assert(0); // TODO: need to switch to a file format that includes object positions
    // scene.AddObject(mesh);

    return scene;
}

class PbrtImporter
{
    struct Token {
        enum class Type {
            Identifier,
            TypeName,
            ParameterList,
        } type;
        const std::string_view value;
    };

    std::string        m_file;
    std::vector<Token> m_tokens;
    size_t             m_index = 0;

public:
    explicit PbrtImporter(const std::string filepath)
    {
        std::ifstream     stream{filepath};
        std::stringstream string_stream;
        string_stream << stream.rdbuf();
        m_file = string_stream.str();
    }

    Scene Import()
    {
        Tokenize();
        return Parse();
    }

private:
    void Tokenize()
    {
        const auto &GetChar = [this] { return m_file[m_index]; };
        while (m_index < m_file.size()) {
            if (GetChar() == '#') {
                for (; m_index < m_file.size() && GetChar() != '\n'; ++m_index)
                    ;
                ++m_index;
            } else {
                Token::Type type;
                if (isupper(GetChar()) || islower(GetChar())) {
                    type = Token::Type::Identifier;
                } else if (isdigit(GetChar()) || GetChar() == '.') {
                    type = Token::Type::ParameterList;
                }
                const size_t token_start = m_index;
                for (; m_index < m_file.size() && !isspace(GetChar()); ++m_index)
                    ;
                const size_t token_end = m_index;
                ++m_index;
                m_tokens.emplace_back({})
            }
        }
    }
    Scene Parse() {}
};

static std::string_view GetFileExtension(const std::string_view filename)
{
    size_t dot_index = filename.find_last_of('.');
    return filename.substr(dot_index + 1);
}

Scene Import(const std::string &path)
{
    if (GetFileExtension(path) == ".obj") {
        return ImportObj(path);
    } else {
        assert(0);
        return Scene{};
    }
}

} // namespace imp
