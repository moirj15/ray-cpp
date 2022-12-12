#include <cstdio>
#include <cstdlib>
#include "window.h"

/**
 * @param w: The width of the window.
 * @param h: The height of the window.
 * @param title: The title of the window.
 * @param c: The canvas that will be displayed on the window.
 */
Window::Window(int32_t w, uint32_t h, const char *title, Canvas *c) {
    width = w;
    height = h;
    canvas = c;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_window = NULL;
    SDL_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, w, h,
                                SDL_WINDOW_SHOWN);

    if (!SDL_window) {
        fprintf(stderr, "ERR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_renderer = SDL_CreateRenderer(SDL_window, -1, 0);

    if (!SDL_renderer) {
        fprintf(stderr, "ERR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_texture = SDL_CreateTexture(SDL_renderer,
                                    SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STATIC, w, h);

    if (!SDL_texture) {
        fprintf(stderr, "ERR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * Destructor.
 */
Window::~Window(void) {
    SDL_DestroyRenderer(SDL_renderer);
    SDL_DestroyTexture(SDL_texture);
    SDL_DestroyWindow(SDL_window);
    delete(canvas);
}

/**
 * Updates what is displayed on the given window.
 *
 * @param win: The window that will be updated.
 */
void Window::updateWindow(void) {
    
    SDL_UpdateTexture(SDL_texture, NULL, canvas->pixels,
                        canvas->width * sizeof(uint32_t));

//    canvas->clearCanvas();
    SDL_RenderClear(SDL_renderer);
    SDL_RenderCopy(SDL_renderer, SDL_texture, NULL, NULL);
    SDL_RenderPresent(SDL_renderer);
}

