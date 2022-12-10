
#include "file_io.hpp"
#include "utils.hpp"

#include <stdio.h>
#include <assert.h>

namespace file_io
{

std::string ReadFile(const std::string &path)
{
    FILE * file = fopen(path.c_str(), "rb");
    assert(file);

    fseek(file, 0, SEEK_END);
    u64 length = ftell(file);
    rewind(file);
    assert(length > 0);

    std::string data(length, 0);

    fread(data.data(), sizeof(u8), length, file);
    fclose(file);
    
    return data;
}

} // namespace file_io
