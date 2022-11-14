#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <glm/glm.hpp>
//#include <glm/mat4x4.hpp>
//#include <glm/vec3.hpp>
//#include <glm/vec4.hpp>

// Some usefule typedefs
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef float  f32;
typedef double f64;

/**
 * Exits the program with EXIT_FAILURE and prints the given
 * error message.
 *
 * @param qMessage: The message to print to stderr.
 */
void quit(const char *qMessage);

/**
 * Opens the given file and handles any errors that may occur.
 * On failure it will print an error message to stderr.
 *
 * @param dir: The directory of the file.
 * @param permission: The permissions to open the file with.
 * @return: The opened file.
 */
FILE *openFile(const char *dir, const char *permission);

/**
 * Closes the given file and sets the pointer to NULL.
 *
 * @param fp: The file to close.
 */
void closeFile(FILE *fp);

template<typename Tag>
class Handle
{
    uint64_t _value = 0;

public:
    static Handle Invalid()
    {
        return Handle();
    }

    Handle() = default;
    explicit Handle(const uint64_t value) : _value(value)
    {
    }

    explicit operator uint64_t() const
    {
        return _value;
    }

    friend bool operator==(Handle a, Handle b)
    {
        return a._value == b._value;
    }

    friend bool operator!=(Handle a, Handle b)
    {
        return a._value != b._value;
    }

    bool operator==(const uint64_t b) const
    {
        return _value == b;
    }

    bool operator!=(const uint64_t b) const
    {
        return _value != b;
    }

    uint64_t operator++(int)
    {
        return _value++;
    }

    size_t Hash() const
    {
        return std::hash<uint64_t>()(_value);
    }
};

namespace std
{

template<typename Tag>
struct hash<Handle<Tag>> {
    size_t operator()(const Handle<Tag> &k) const
    {
        return k.Hash();
    }
};
} // namespace std
#define MAKE_HANDLE(name)                                                                                                                                      \
    struct name##Tag {                                                                                                                                         \
    };                                                                                                                                                         \
    using name = Handle<name##Tag>

#endif
