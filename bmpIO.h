#ifndef BMPIO_H
#define BMPIO_H

#include "utils.h"

/**
 * Converts a vec4 containing color data to a u32 with the same color data.
 *
 * @param color: The vector with the color data.
 * @return: The color data as a u32.
 */
u32 vec4_to_u32(const glm::vec4 color);

/**
 * Writes a bmp file out to the given file name.
 *
 * @param file: The file that will be written out.
 * @param data: The color data that will be stored in the file.
 * @param size: The size of data.
 */
void writeBMP(const char *file, glm::vec4 *image, u32 width, u32 height);

#endif
