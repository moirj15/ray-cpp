#include "bmpIO.hpp"

#include "tracer.hpp"

#include <SDL2/SDL.h>
#include <vector>

namespace bmp
{

/**
 * Converts a vec4 containing color data to a u32 with the same color data.
 *
 * @param color: The vector with the color data.
 * @return: The color data as a u32.
 */
u32 vec4_to_u32(const glm::vec3 &color)
{
    u8 red = (u8)(255.0 * color.r);
    u8 green = (u8)(255.0 * color.g);
    u8 blue = (u8)(255.0 * color.b);

    return (0xff << 24) | (blue << 16) | (green << 8) | red;
}

/**
 * Writes a bmp file out to the given file name.
 *
 * @param file: The file that will be written out.
 * @param image: The image that will be created.
 */
void Write(const char *file, const Frame &frame)
{
    u32 rmask = 0x000000ff;
    u32 gmask = 0x0000ff00;
    u32 bmask = 0x00ff0000;
    u32 amask = 0xff000000;

    std::vector<u32> data;
    for (u32 i = 0; i < frame.GetWidth() * frame.GetHeight(); i++) {
        data.push_back(vec4_to_u32(frame.GetImage()[i]));
    }

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
        (void *)data.data(), frame.GetWidth(), frame.GetHeight(), 32, frame.GetWidth() * 4, rmask, gmask, bmask, amask);

    if (SDL_SaveBMP(surface, file) < 0) {
        printf("SDL failed to save file: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surface);
}

} // namespace bmp
