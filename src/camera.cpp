#include "camera.h"

#include "bmpIO.h"

#include <cmath>
#include <glm/gtx/string_cast.hpp>

void Camera::render(const Scene &w)
{
    const auto &objects = w.objList;
    for (s32 y = 0; y < HEIGHT; y++) {
        f32 filmPlanY = (_film_plane_height / 2) - (y * (_film_plane_height / HEIGHT));
        for (s32 x = 0; x < WIDTH; x++) {
            f32 filmPlanX = (-_film_plane_width / 2) + (x * (_film_plane_width / WIDTH));
            glm::vec4 direction(filmPlanX, filmPlanY, -_f, 0.0);

            Ray r(glm::normalize(glm::vec4(_eyepoint, 1.0)), glm::normalize(direction));

            IntersectData data;
            s32 obj = intersection(w, r, data, -1);

            if (obj == -1) {
                _frame.SetPixel(x, y, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) * lmax);
            } else {
                glm::vec4 color = calculateLight(w, data, objects[obj]->GetIlluminationModel(), w.lightList, obj, 1);
                _frame.SetPixel(x, y, color * lmax);
            }
        }
    }
    tone_rep(REINHARD);
}

s32 Camera::intersection(const Scene &world, Ray ray, IntersectData &data, s32 check_obj)
{
    const auto &objects = world.objList;
    IntersectData closest_data;
    f32 closest_distance = INFINITY;
    f32 test_distance;
    s32 obj = -1;
    for (u64 i = 0; i < objects.size(); i++) {

        IntersectData curr_data;

        if (objects[i]->Intersect(ray, curr_data)) {
            test_distance = glm::distance(_eyepoint, curr_data.intersection);
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

#define MAX_DEPTH 4
static const f32 refract_air = 1.0f;
static const f32 refract_glass = 1.1f;

glm::vec4 Camera::calculateLight(const Scene &world, IntersectData &id, const IlluminationModel &i_model,
    std::vector<Light> lights, s32 obj, u32 depth)
{
    glm::vec4 color(0.0);
    for (u64 i = 0; i < lights.size(); i++) {
        Light &light = lights[i];

        color += i_model.illuminate(id, light, _eyepoint, inShadow(world, id, light, obj));

        if (depth < MAX_DEPTH) {
            if (i_model._reflection_const > 0.0) {
                const auto surfToLight = glm::normalize(_eyepoint - id.intersection);
                const auto reflectionVec = glm::normalize(-glm::reflect(surfToLight, id.normal));
                Ray ray(id.intersection, reflectionVec);
                IntersectData data;
                s32 hit_obj = intersection(world, ray, data, obj);

                if (hit_obj > -1) {
                    color += i_model._reflection_const
                             * calculateLight(world, data, world.objList[hit_obj]->GetIlluminationModel(), lights,
                                 hit_obj, depth + 1);
                } else {
                    color += i_model._reflection_const * glm::vec4(0.0, 0.0, 1.0, 0.0);
                }
            }
            if (i_model._refraction_const > 0.0) {
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
                s32 hit_obj = intersection(world, transmission, data, -1);
                if (hit_obj > -1) {
                    color += i_model._refraction_const
                             * calculateLight(world, data, world.objList[hit_obj]->GetIlluminationModel(), lights,
                                 hit_obj, depth + 1);
                } else {
                    color += i_model._refraction_const * glm::vec4(0.0, 0.0, 1.0, 0.0);
                }
            }
        }
    }

    return color;
}

bool Camera::inShadow(const Scene &world, IntersectData &id, Light light, s32 obj)
{
    Ray r;
    r.direction = glm::normalize(light.position - id.intersection);
    r.origin = id.intersection;

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
    return false;
}

void Camera::tone_rep(TONE_TYPE which_type)
{
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
            _frame.SetPixel(i, glm::clamp(pixel, glm::vec4(0.0f), glm::vec4(1.0f)));
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
            _frame.SetPixel(i, glm::clamp(pixel, glm::vec4(0.0f), glm::vec4(1.0f)));
        }
    }
}
