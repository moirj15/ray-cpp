#pragma once

#include <glm/vec4.hpp>

class Material
{
  public:
    virtual ~Material() = default;

    [[nodiscard]] glm::vec4 Sample() = 0;
};