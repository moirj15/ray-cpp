#ifndef CAMERA_H
#define CAMERA_H
#include "utils.h"
#include "Scene.h"
#include "intersectData.h"
#include "illumination.h"

#define WIDTH 	1080
#define HEIGHT	720

#define PIC_SIZE (WIDTH * HEIGHT)

enum TONE_TYPE
{
	WARD,
    REINHARD
};

class Camera
{

	glm::vec3 eyepoint;
	glm::vec3 lookAt;
	glm::vec3 up;

	f32 filmPlaneWidth;
	f32 filmPlaneHeight;
	f32	f;

    const f32 ldmax = 1.0f;
    f32 lmax;

public:
	glm::mat4 camTransform;
    glm::vec4 picture[PIC_SIZE];

    /**
     *
     */
	Camera(void);

    /**
     *
     */
	Camera(glm::vec3 e, glm::vec3 l, glm::vec3 u, f32 fp_w, f32 fp_h, f32 lm);

    /**
     *
     */
	~Camera(void);


    /**
     *
     */
	void render(const Scene &w);

    /**
     *
     */
	s32 intersection(const Scene &world, Ray ray, IntersectData &data, s32 check_obj);
private:

    /**
     *
     */
	glm::vec4 calculateLight(const Scene &world,IntersectData &id, const IlluminationModel &i_model, 
			std::vector<Light> lights, s32 obj, u32 depth);

    /**
     *
     */
	bool inShadow(const Scene &world, IntersectData &id, Light light, s32 obj);

    /**
     *
     */
    void tone_rep(TONE_TYPE which_type);
};


#endif
