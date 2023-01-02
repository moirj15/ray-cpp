#include "canvas.h"
#include "pipeline.h"
#include "window.h"

#include <SDL2/SDL.h>
#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "obj.h"

#include <glm/glm.hpp>
// #include <glm/ext.hpp>
#include "../Camera.h"
#include "../geometry/scene.hpp"
#include "../importer/scene_importer.hpp"
#include "SoftwareRenderer.hpp"
#include "pipeline.h"

#include <glm/gtx/transform.hpp>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    constexpr u32 WIDTH  = 1920;
    constexpr u32 HEIGHT = 1080;

    Camera               camera{glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)};
    ra::SoftwareRenderer renderer(WIDTH, HEIGHT, camera);

    assert(SDL_Init(SDL_INIT_VIDEO) == 0);
    SDL_Window *window = SDL_CreateWindow("Software Rasterizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    assert(window != nullptr);

    // Don't resize window so it doesn't mess with tiling window managers
    SDL_SetWindowResizable(window, SDL_FALSE);

    SDL_Renderer *sdl_renderer = SDL_CreateRenderer(window, -1, 0);

    assert(sdl_renderer != nullptr);

    SDL_Texture *sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);

    assert(sdl_texture != nullptr);

    Importer importer{"test-obj/block.obj"};
    Scene    scene = importer.Import();

    renderer.Draw();

    SDL_UpdateTexture(sdl_texture, nullptr, renderer.GetFrameBuffer().GetBuffer(), renderer.GetFrameBuffer().GetWidth() * sizeof(uint32_t));

    //    canvas->clearCanvas();
    SDL_RenderClear(sdl_renderer);
    SDL_RenderCopy(sdl_renderer, sdl_texture, nullptr, nullptr);
    SDL_RenderPresent(sdl_renderer);

    SDL_Event e;
    bool      running = true;
    while (running) {
        while (SDL_PollEvent(&e) > 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
    }

#if 0
    Canvas c{640, 480};

    Window win{640, 480, "engine", c};

    win->updateWindow();
    c->clearCanvas();
    win->updateWindow();

    Light light = {glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)};

    Camera *camera = new Camera(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0), 640, 480, 70.0f, 0.1f, 1000.0f);

    std::vector<glm::vec4> vertexList;

    vertexList.push_back(glm::vec4(-1.0, -1.0, 0.0, 1.0));
    vertexList.push_back(glm::vec4(0.0, 1.0, 0.0, 1.0));
    vertexList.push_back(glm::vec4(1.0, -1.0, 0.0, 1.0));
    /*
        vertexList.push_back(glm::vec4( 0.50, 0.75, -0.5, 1.0));	// 0
        vertexList.push_back(glm::vec4( 0.50, 0.50, -0.5, 1.0));	// 1
        vertexList.push_back(glm::vec4( 1.0, 0.50, -0.5 ,1.0));	// 2
        vertexList.push_back(glm::vec4( 1.0, 0.75, -0.5 ,1.0));	// 3

        vertexList.push_back(glm::vec4(0.50, 0.75, -1.0, 1.0));		// 4
        vertexList.push_back(glm::vec4(1.0, 0.75, -1.0, 1.0));	// 5
        */
    std::vector<u64> connections;

    connections.push_back(0);
    connections.push_back(1);
    connections.push_back(2);
    /*
        connections.push_back(0);
        connections.push_back(2);
        connections.push_back(3);

        connections.push_back(4); // 4
        connections.push_back(0); // 0
        connections.push_back(3); // 3

        connections.push_back(4);
        connections.push_back(3);
        connections.push_back(5);
    */
    VBO vbo(vertexList, connections);
    // VBO vbo = readOBJ("test_obj/plate.obj");
    win->updateWindow();

    u32 lastTime = 0;
    s32 nbFrames = 0;

    SDL_Event e;
    bool      running = true;
    float     ang     = 0.0;
    u32       seconds = 0;
    Material  mat     = {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.5f, 0.5f, 0.5f, 70.0f};
    while (running) {
        // Polygon p2 = clipPolygon(poly, glm::vec2(-1.0, -1.0),
        //	glm::vec2(1.0, 1.0));

        win->updateWindow();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
        const uint8_t *key_event = SDL_GetKeyboardState(NULL);
        if (key_event[SDL_SCANCODE_Q]) {
            running = false;
        }

        nbFrames++;
        s32 currentTime = SDL_GetTicks();
        u32 diff        = currentTime - lastTime;
        ang += 0.001;
        glm::mat4 t      = glm::translate(glm::vec3(0.0f, 0.0f, -10.0f));
        camera->modelMat = t * glm::rotate(ang, glm::vec3(1.0f, 0.0f, 0.0f));

        std::vector<u32> colors;
        VBO              poly = transform_to_2D(vbo, camera, light, mat, colors);
        // poly.print();
        // Polygon p2 = clipPolygon(poly, glm::vec2(0.0, 0.0), glm::vec2(640.0, 480.0));
        // p2.print();

        c->clearCanvas();
        draw_vbo(poly, c, colors);
        /*for (u64 i = 0; i < poly.size(); i++) {
                fillPolygon(poly[i], c, colors[i]);
}*/

        // TODO: make sure this is calculating the frame rate correctly
        if (diff > 1000) {
            seconds++;
            // double msFrame = 1000.0 / (double) nbFrames;
            // printf("%f ms/frame\n", msFrame);
            printf("%d fps\n", nbFrames);
            nbFrames = 0;
            lastTime = SDL_GetTicks();
        }
        if (seconds > 1) {
            // running = false;
        }
    }

    delete (win);
#endif

    return 0;
}
