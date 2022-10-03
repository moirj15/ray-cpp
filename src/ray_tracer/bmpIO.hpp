#pragma once

class Frame;

namespace bmp
{

/**
 * Writes a bmp file out to the given file name.
 *
 * @param file: The file that will be written out.
 * @param data: The color data that will be stored in the file.
 * @param size: The size of data.
 */
void Write(const char *file, const Frame &frame);

}


