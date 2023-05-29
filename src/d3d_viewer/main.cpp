#include "../utils.hpp"
#include "SceneViewer.hpp"
#include "../geometry/scene.hpp"
#include "../importer/scene_importer.hpp"
#include "../Camera.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <assert.h>

constexpr u32 WIDTH  = 1920;
constexpr u32 HEIGHT = 1080;

int main(int argc, char **argv)
{
    assert(SDL_Init(SDL_INIT_VIDEO) == 0);
    SDL_Window *window = SDL_CreateWindow("D3D Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    assert(window != nullptr);

    // Don't resize window so it doesn't mess with tiling window managers
    SDL_SetWindowResizable(window, SDL_FALSE);

    Camera camera{glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)};
    Scene scene = imp::Import("test-obj/block.obj");

    SDL_SysWMinfo win_info;
    SDL_VERSION(&win_info.version);
    SDL_GetWindowWMInfo(window, &win_info);
    HWND hwnd = win_info.info.win.window;

    sv::SceneViewer scene_viewer{Window{hwnd, WIDTH, HEIGHT}, scene, camera};
    scene_viewer.SetupScene(scene);

    SDL_Event e;
    bool      running = true;
    while (running) {
        while (SDL_PollEvent(&e) > 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
        scene_viewer.Tick();
        scene_viewer.SwapBuffers();

    }

    return 0;
}
