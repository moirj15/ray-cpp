#include <SDL2/SDL.h>

#include <assert.h>
#include "../utils.hpp"

constexpr u32 WIDTH = 1920;
constexpr u32 HEIGHT = 1080;

int main(int argc, char **argv) {
    SDL_Window *window =
        SDL_CreateWindow("D3D Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    assert(window != nullptr);

    // Don't resize window so it doesn't mess with tiling window managers
    SDL_SetWindowResizable(window, SDL_FALSE);
    return 0;
}
