#pragma once

#include "../geometry/scene.hpp"
#include <string>
#include <string_view>

#if 0
class Importer
{
    std::string m_file;
    size_t m_index = 0;
public:
    explicit Importer(const std::string &path);

    Scene Import();

private:
    std::string_view ConsumeToken();

    void SkipLine();
};
#endif

class Importer
{
    std::string m_filepath;

public:
    explicit Importer(const std::string &path);

    Scene Import();
};
