#include "tracer.hpp"

#include "bmpIO.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "scene.hpp"

#include <cmath>
#include <glm/gtx/string_cast.hpp>

namespace tracer
{

void RenderFrame(const Scene &scene, const Camera &camera, Frame &frame)
{
    std::vector<Ray> initial_rays;
    // TODO: This forces the origin ray to be coming from the top left of the screen
    //    for (s32 y = _frame.GetHeight() - 1; y >= 0 ; y--) {
    for (s32 y = 0; y < frame.GetHeight(); y++) {
        f32 filmPlanY = (camera.film_plane_height / 2)
                        - (static_cast<f32>(y) * (camera.film_plane_height / static_cast<f32>(frame.GetHeight())));
        for (s32 x = 0; x < frame.GetWidth(); x++) {
            f32 filmPlanX = (-camera.film_plane_width / 2)
                            + (static_cast<f32>(x) * (camera.film_plane_width / static_cast<f32>(frame.GetWidth())));
            glm::vec3 direction(filmPlanX, filmPlanY, -camera.f);
            initial_rays.emplace_back(camera.eyepoint, glm::normalize(direction));
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
        Object *last_hit = nullptr;
        for (u32 d = 0; d < depth; d++) {
            for (const auto &ray : bounce_rays) {
                IntersectData intersect_data;
                intersect_data.hit_obj = last_hit;
                auto *object = scene.CastRay(ray, intersect_data, -1);
                last_hit = object;

                if (!object) {
                    //                    _frame.SetPixel(i, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)/* * lmax*/);
                    continue;
                }
                no_hits = false;
//                color += glm::abs(intersect_data.normal); //+= object->GetShader().Execute(intersect_data);
                color += object->GetShader().Execute(intersect_data);

                for (const auto &light : scene.GetLights()) {
                    const auto surfToLight = glm::normalize(camera.eyepoint - intersect_data.intersection);
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
        frame.SetPixel(i, glm::clamp(color, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}));
    }
#if 0 //TODO
    tone_rep(REINHARD);
#endif
}

#if 0
s32 Camera::Intersection(const Scene &world, Ray ray, IntersectData &data, s32 check_obj)
{
    const auto &objects = world.objList;
    IntersectData closest_data;
    f32 closest_distance = INFINITY;
    f32 test_distance;
    s32 obj = -1;
    for (u64 i = 0; i < objects.size(); i++) {

        IntersectData curr_data;

        if (objects[i]->Intersect(ray, curr_data)) {
            test_distance = glm::distance(eyepoint, curr_data.intersection);
            if ((check_obj > -1) && (i == (u64)check_obj)) {
                continue;
            }
            if (test_distance <= closest_distance) {
                closest_distance = test_distance;
                obj = (s32)i;
                closest_data = curr_data;
            }
        }
    }
    data = closest_data;
    return obj;
}
#endif

#define MAX_DEPTH 4
static const f32 refract_air = 1.0f;
static const f32 refract_glass = 1.1f;

glm::vec4 CalculateLight(
    const Scene &scene, IntersectData &id, const Shader &i_model, std::vector<Light> lights, s32 obj, u32 depth)
{
    glm::vec4 color(1.0);
#if 0
    glm::vec4 color(0.0);
    for (u64 i = 0; i < lights.size(); i++) {
        Light &light = lights[i];

        color += _shader.illuminate(id, light, eyepoint, inShadow(scene, id, light, obj));

        if (depth < MAX_DEPTH) {
            if (_shader._reflection_const > 0.0) {
                const auto surfToLight = glm::normalize(eyepoint - id.intersection);
                const auto reflectionVec = glm::normalize(-glm::reflect(surfToLight, id.normal));
                Ray ray(id.intersection, reflectionVec);
                IntersectData data;
                s32 hit_obj = scene.CastRay(scene, ray, data, obj);

                if (hit_obj > -1) {
                    color += _shader._reflection_const
                             * CalculateLight(scene, data, scene._objects[hit_obj]->GetIlluminationModel(), lights,
                                 hit_obj, depth + 1);
                } else {
                    color += _shader._reflection_const * glm::vec4(0.0, 0.0, 1.0, 0.0);
                }
            }
            if (_shader._refraction_const > 0.0) {
                f32 ni, nt;
                glm::vec3 normal, direction;
                const auto ray_dir = glm::normalize(id.ray.direction);
                if (glm::dot(id.normal, ray_dir) < 0.0) {
                    ni = refract_glass;
                    nt = refract_air;
                    normal = (-id.normal);
                } else {
                    ni = refract_air;
                    nt = refract_glass;
                    normal = (id.normal);
                }

                f32 internal_ref_check =
                    1.0 - ((powf(ni, 2.0) * (1.0 - powf(glm::dot(ray_dir, normal), 2.0))) / powf(nt, 2.0));

                if (refract_glass == refract_air) {
                    direction = ray_dir;
                } else if (internal_ref_check < 0.0) {
                    // Total internal reflection
                    direction = glm::normalize(glm::reflect(id.intersection, normal));
                } else {
                    direction = glm::normalize(((ni * (ray_dir - normal * glm::dot(ray_dir, normal))) / nt)
                                               + (normal * sqrtf(internal_ref_check)));
                }

                Ray transmission(id.intersection + (0.1f * normal), direction);
                IntersectData data;
                s32 hit_obj = Intersection(scene, transmission, data, -1);
                if (hit_obj > -1) {
                    color += _shader._refraction_const
                             * CalculateLight(scene, data, scene._objects[hit_obj]->GetIlluminationModel(), lights,
                                 hit_obj, depth + 1);
                } else {
                    color += _shader._refraction_const * glm::vec4(0.0, 0.0, 1.0, 0.0);
                }
            }
        }
    }
#endif

    return color;
}

bool inShadow(const Scene &world, IntersectData &id, Light light, s32 obj)
{
    Ray r;
    r.direction = glm::normalize(light.position - id.intersection);
    r.origin = id.intersection;

#if 0
    const auto &objects = world.objList;
    IntersectData dummy;
    for (u64 i = 0; i < objects.size(); i++) {
        if (obj == (s32)i) {
            continue;
        }
        if (objects[i]->Intersect(r, dummy)) {
            return true;
        }
    }
#endif
    return false;
}

void tone_rep(TONE_TYPE which_type)
{
#if 0
    f32 log_avg_luminance = 0.0f;
    for (u64 i = 0; i < PIC_SIZE; i++) {
        auto pixel = _frame.GetPixel(i);
        log_avg_luminance += log(0.1f + 0.27f * pixel.r + 0.67f * pixel.g + 0.06f * pixel.b);
    }
    log_avg_luminance /= PIC_SIZE;
    log_avg_luminance = exp(log_avg_luminance);

    if (which_type == WARD) {
        f32 world_illum = log_avg_luminance;
        printf("%f\n", world_illum);
        f32 sf = powf((1.219f + powf(ldmax / 2.0f, 0.4f)) / (1.219f + powf(world_illum, 0.4f)), 2.5f);
        for (s32 i = 0; i < PIC_SIZE; i++) {
            auto pixel = _frame.GetPixel(i);
            pixel *= sf;
            pixel.r /= ldmax;
            pixel.g /= ldmax;
            pixel.b /= ldmax;
            _frame.SetPixel(i, glm::clamp(pixel, glm::vec3(0.0f), glm::vec3(1.0f)));
        }
    } else if (which_type == REINHARD) {
        f32 a = 0.18f;
        for (s32 i = 0; i < PIC_SIZE; i++) {
            auto pixel = _frame.GetPixel(i);
            pixel *= a / log_avg_luminance;
            pixel.r /= 1.0f + pixel.r;
            pixel.g /= 1.0f + pixel.g;
            pixel.b /= 1.0f + pixel.b;
            // _picture[i] *= ldmax;
            _frame.SetPixel(i, glm::clamp(pixel, glm::vec3(0.0f), glm::vec3(1.0f)));
        }
    }
#endif
}
} // namespace tracer
