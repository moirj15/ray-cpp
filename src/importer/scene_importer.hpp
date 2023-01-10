#pragma once

#include "../geometry/scene.hpp"

#include <string>
#include <string_view>


namespace imp
{

Scene Import(const std::string &path);

} // namespace imp
