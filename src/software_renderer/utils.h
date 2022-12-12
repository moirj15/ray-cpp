#ifndef UTILS_H
#define  UTILS_H

#include <stdint.h>
#include <cstdio>
#include <vector>

// uncomment for laptop
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#define local static
#define db() printf("hit\n");

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;

typedef float		f32;
typedef double		f64;


/**
 * Exits the program with EXIT_FAILURE and prints the given 
 * error message.
 *
 * @param qMessage: The message to print to stderr.
 */
void quit(const char *qMessage);


#if defined(_WIN32) || defined(_WIN64)
/**
 * Implementation of the getline function provided in GCC.
 *
 * @param lineptr: Pointer to a string, will be re-allocated as needed.
 * @param n: The size of the string buffer.
 * @param stream: The file that will be read from.
 * @return: Number of characters read on success, otherwise -1.
 */
s32 getline(char **lineptr, u64 *n, FILE *stream);
#endif
#endif // UTILS_H
