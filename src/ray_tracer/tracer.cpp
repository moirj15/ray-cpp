#include "tracer.hpp"

#include "bmpIO.hpp"
#include "camera.hpp"
#include "../geometry/object.hpp"
#include "../geometry/scene.hpp"
#include "frame.hpp"
#include "../math/ray.hpp"

#include <cmath>
#include <glm/gtx/string_cast.hpp>


void Tracer::RenderFrame()
{
    std::vector<Ray> initial_rays;
    // TODO: This forces the origin ray to be coming from the top left of the screen
    //    for (s32 y = _frame.GetHeight() - 1; y >= 0 ; y--) {
    for (s32 y = 0; y < m_frame.GetHeight(); y++) {
        f32 filmPlanY = (m_camera.film_plane_height / 2)
                        - (static_cast<f32>(y) * (m_camera.film_plane_height / static_cast<f32>(m_frame.GetHeight())));
        for (s32 x = 0; x < m_frame.GetWidth(); x++) {
            f32 filmPlanX = (-m_camera.film_plane_width / 2)
                            + (static_cast<f32>(x) * (m_camera.film_plane_width / static_cast<f32>(m_frame.GetWidth())));
            glm::vec3 direction(filmPlanX, filmPlanY, -m_camera.f);
            initial_rays.emplace_back(m_camera.eyepoint, glm::normalize(direction));
            //            initial_rays.emplace_back(direction, look_at);
        }
    }
    const u32 depth = 2;
    for (s32 i = 0; i < initial_rays.size(); i++) {
        glm::vec3 color(0.0, 0.0, 0.0);
        std::vector<Ray> bounce_rays;
        bounce_rays.push_back(initial_rays[i]);
        std::vector<Ray> next_bounce;
        bool no_hits = true;
        // TODO: Find a better method to prevent self intersections
        Object *last_hit = nullptr;
        for (u32 d = 0; d < depth; d++) {
            for (const auto &ray : bounce_rays) {
                IntersectData intersect_data;
                intersect_data.hit_obj = last_hit;
                auto *object = m_scene.CastRay(ray, intersect_data, -1);
                last_hit = object;

                if (!object) {
                    //                    _frame.SetPixel(i, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)/* * lmax*/);
                    continue;
                }
                no_hits = false;
//                color += glm::abs(intersect_data.normal); //+= object->GetShader().Execute(intersect_data);
                color += object->GetShader().Execute(intersect_data);

                for (const auto &light : m_scene.GetLights()) {
                    const auto surfToLight = glm::normalize(m_camera.eyepoint - intersect_data.intersection);
                    const auto reflectionVec = glm::normalize(-glm::reflect(surfToLight, intersect_data.normal));
                    next_bounce.emplace_back(intersect_data.intersection, reflectionVec);
                }
            }
            std::swap(bounce_rays, next_bounce);
            next_bounce.clear();
        }
        if (no_hits) {
            color = {0, 0, 1};
        }
        m_frame.SetPixel(i, glm::clamp(color, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}));
    }
}