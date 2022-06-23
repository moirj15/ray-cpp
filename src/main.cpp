#include "SDL2/SDL.h"
#include "bmpIO.h"
#include "camera.h"
#include "material.h"
#include "object.h"
#include "scene.h"
#include "shader.h"
#include "utils.h"
// #include "SceneViewer.h"

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

static const glm::vec4 ambient_mat(1.0, 1.0, 1.0, 1.0);
static const glm::vec4 diffuse_mat(1.0, 1.0, 1.0, 1.0);
static const glm::vec4 specular_mat(1.0, 1.0, 1.0, 1.0);

static const f32 film_plane_width = 16.0f;
static const f32 film_plane_height = 9.0f;
static const f32 ldmax = 1.0f;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("start\n");
    Scene scene;

    const Light light(
        glm::vec4(0.0f, 0.00f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    scene.AddLight(light);

    Phong sphere_illum1(scene, 0.0f, 0.5f, 0.5f, 70.0f, 0.0f, 0.8f, ambient_mat, diffuse_mat, specular_mat);
    Phong sphere_illum2(scene, 0.0f, 0.5f, 0.5f, 70.0f, 0.5f, 0.0f, ambient_mat, diffuse_mat, specular_mat);

    glm::vec4 color1(1.0, 0.0, 0.0, 0.0);
    glm::vec4 color2(1.0, 1.0, 0.0, 0.0);

    CheckerBoard floor_illum(scene, 0.5, 0.5, 0.5, 70.0, 0.0, 0.0, color1, color2);

    std::vector<glm::vec3> triangles;
    #if 0
    triangles.emplace_back(-1.0, 0.0, -1.0); // fl
    triangles.emplace_back(-1.0, 0.0, 1.0);  // bl
    triangles.emplace_back(1.0, 0.0, 1.0);   // br

    triangles.emplace_back(1.0, 0.0, 1.0);   // br
    triangles.emplace_back(1.0, 0.0, -1.0);  // tr
    triangles.emplace_back(-1.0, 0.0, -1.0); // fl
    #endif

    triangles.emplace_back(-1.0, 0.0, -1.0); // fl
    triangles.emplace_back(-1.0, 0.0, 1.0);  // bl
    triangles.emplace_back(1.0, 0.0, 1.0);   // br
    triangles.emplace_back(1.0, 0.0, -1.0);  // tr

    std::vector<u32> indices = {
        0, 1, 2,
        0, 2, 3,
    };


    for (auto &p : triangles) {
        auto t = glm::translate(glm::mat4(1.0), {0.0, -1.9, 0.0});
        p = glm::scale(t, {10, 5, 10}) * glm::vec4(p, 1.0);
    }

    auto plane_transform = glm::scale(glm::translate(glm::mat4(1.0), {0.0, -1.9, 0.0}), {10, 5, 10});

    scene.AddObject(new Mesh(triangles, indices, &floor_illum), plane_transform);

    scene.AddObject(new Sphere(glm::vec4(-1.0f, 0.0f, 0.4f, 1.0f), 1.00f, &sphere_illum1));
    scene.AddObject(new Sphere(glm::vec4(0.3f, 0.3f, -0.3f, 1.0), 0.8, &sphere_illum2));

    const glm::vec3 camera_pos(0.0f, 0.0f, 1.0f);
    const glm::vec3 look_at(0.0f, 0.0f, -1.0f);
    const glm::vec3 up_vec(0.0f, 1.0f, 0.0f);
    Camera camera(camera_pos, look_at, up_vec, film_plane_width, film_plane_height, 1.0f);
    Frame frame(600, 400);

    // SceneViewer scene_viewer(scene, camera);
    // Window window = scene_viewer.MakeWindow("preview", 1920, 1080);
//    SDL_Event e;
//    bool running = true;
//    while (running) {
//        while (SDL_PollEvent(&e) > 0) {
//            if (e.type == SDL_QUIT) {
//                running = false;
//            }
//        }
//    }

    // Getting the gl-preview working
    //    auto ray_thread = std::thread([&scene, &camera]() {
    scene.Transform(camera.camera_transform);
    tracer::Render(scene, camera, frame);

    bmp::Write("test.bmp", frame);
    printf("finished\n");
#if 0
    //    });

    //    SDL_Event e;
    //    bool running = true;
    //    auto *device = focus::Device::Init(focus::RendererAPI::OpenGL);
    //    auto window = device->MakeWindow(1920, 1080);
    //    while (running) {
    //        while (SDL_PollEvent(&e) > 0) {
    //            if (e.type == SDL_QUIT) {
    //                running = false;
    //            }
    //        }
    //    }
    //
    //    ray_thread.join();
#endif

    return EXIT_SUCCESS;
}
