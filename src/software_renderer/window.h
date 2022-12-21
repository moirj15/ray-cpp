#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>

#include "canvas.h"

namespace ra
{

struct Window {
    SDL_Window      *SDL_window;
    SDL_Texture     *SDL_texture;
    SDL_Renderer    *SDL_renderer;
    Canvas          *canvas;
    uint32_t        width;
    uint32_t        height;

	/**
 	 * @param w: The width of the window.
 	 * @param h: The height of the window.
 	 * @param title: The title of the window.
 	 * @param c: The canvas that will be displayed on the window.
 	 */
	Window(int32_t w, uint32_t h, const char *title, Canvas *c);

	/**
	 * Destructor.
	 */
	~Window(void);

	/**
	* Updates what is displayed on the given window.
	*
	* @param win: The window that will be updated.
	*/
	void updateWindow(void);
};
}