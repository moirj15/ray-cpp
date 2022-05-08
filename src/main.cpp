#include "SDL2/SDL.h"
#include "bmpIO.h"
#include "camera.h"
#include "material.h"
#include "object.h"
#include "scene.h"
#include "shader.h"
#include "utils.h"

#include <cstdio>
#include <cstdlib>
#include <focus.hpp>
#include <thread>

static const glm::vec4 ambient_mat(1.0, 1.0, 1.0, 1.0);
static const glm::vec4 diffuse_mat(1.0, 1.0, 1.0, 1.0);
static const glm::vec4 specular_mat(1.0, 1.0, 1.0, 1.0);

static const f32 film_plane_width = 16.0f;
static const f32 film_plane_height = 9.0f;
static const f32 ldmax = 1.0f;

class SceneViewer
{
    focus::Device *m_device;
    focus::VertexBuffer m_vertex_buffer;
    focus::Pipeline m_pipeline;

    const Scene &m_scene;
    const Camera &m_camera;

    struct ObjectConstants {
        glm::mat4 mvp;
        glm::vec3 color;
    };

    std::vector<ObjectConstants> m_constants;
    std::vector<glm::vec3> m_vertices;

  public:
    explicit SceneViewer(const Scene &scene, const Camera &camera) : m_scene(scene), m_camera(camera)
    {
        focus::Device::Init(focus::RendererAPI::OpenGL);
        for (const auto &object : m_scene.GetObjects()) {
            switch (object->type) {
            case Object::Type::Sphere:
                BuildSphere(reinterpret_cast<Sphere *>(object.get()));
                break;
            case Object::Type::Mesh:
                AddMesh(reinterpret_cast<Mesh *>(object.get()));
                break;
            }
        }
    }

  private:
    void BuildSphere(const Sphere *sphere) {}

    void AddMesh(const Mesh *mesh)
    {
        for (const auto &vertex : mesh->GetVertices()) {
            m_vertices.emplace_back(vertex);
        }
        // TODO: store matrix with objects?
        // m_constants.emplace_back(m_camera.GetCameraTransform() * )
    }
};

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

    triangles.emplace_back(-1.0, 0.0, -1.0); // fl
    triangles.emplace_back(-1.0, 0.0, 1.0);  // bl
    triangles.emplace_back(1.0, 0.0, 1.0);   // br

    triangles.emplace_back(1.0, 0.0, 1.0);   // br
    triangles.emplace_back(1.0, 0.0, -1.0);  // tr
    triangles.emplace_back(-1.0, 0.0, -1.0); // fl

    for (auto &p : triangles) {
        auto t = glm::translate(glm::mat4(1.0), {0.0, -1.9, 0.0});
        p = glm::scale(t, {10, 5, 10}) * glm::vec4(p, 1.0);
    }

    auto plane_transform = glm::scale(glm::translate(glm::mat4(1.0), {0.0, -1.9, 0.0}), {10, 5, 10});

    scene.AddObject(new Mesh(triangles, &floor_illum), plane_transform);

    scene.AddObject(new Sphere(glm::vec4(-1.0f, 0.0f, 0.4f, 1.0f), 1.00f, &sphere_illum1));
    scene.AddObject(new Sphere(glm::vec4(0.3f, 0.3f, -0.3f, 1.0), 0.8, &sphere_illum2));

    const glm::vec3 camera_pos(0.0f, 0.0f, 1.0f);
    const glm::vec3 look_at(0.0f, 0.0f, -1.0f);
    const glm::vec3 up_vec(0.0f, 1.0f, 0.0f);
    Camera camera(camera_pos, look_at, up_vec, film_plane_width, film_plane_height, 1.0f);

    //    auto ray_thread = std::thread([&scene, &camera]() {
    scene.Transform(camera.GetCameraTransform());
    camera.Render(scene);

    const auto &frame = camera.GetFrame();
    bmp::Write("test.bmp", frame);
    printf("finished\n");
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

    return EXIT_SUCCESS;
}
