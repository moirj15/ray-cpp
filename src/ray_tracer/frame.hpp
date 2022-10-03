#pragma once

#include "../utils.hpp"
#include <vector>
#include <glm/vec3.hpp>
class Frame
{
    const s32 m_width;
    const s32 m_height;
    std::vector<glm::vec3> m_image;

public:
    Frame(s32 width, s32 height) : m_width(width), m_height(height), m_image(m_width * m_height, glm::vec4(0)) {}
    void SetPixel(s32 x, s32 y, const glm::vec3 &color) { m_image[m_width * y + x] = color; }
    void SetPixel(s32 i, const glm::vec3 &color) { m_image[i] = color; }
    [[nodiscard]] glm::vec3 GetPixel(s32 x, s32 y) const { return m_image[m_width * y + x]; }
    [[nodiscard]] glm::vec3 GetPixel(s32 i) const { return m_image[i]; }
    [[nodiscard]] s32 GetWidth() const { return m_width; }
    [[nodiscard]] s32 GetHeight() const { return m_height; }
    [[nodiscard]] const std::vector<glm::vec3> &GetImage() const { return m_image; }
};
