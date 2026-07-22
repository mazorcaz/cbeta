// camera.h

#ifndef CBETA_CAMERA_H
#define CBETA_CAMERA_H

#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

struct cb_camera {
	float x, y, z;
	float yaw, pitch;
	float speed, sensitivity;
};

void cb_camera_init(struct cb_camera* camera);
void cb_camera_handle_mouse(struct cb_camera* camera, float dx, float dy);
void cb_camera_handle_keys(struct cb_camera* camera, const uint8_t* keys, uint64_t dt);
void cb_camera_apply(struct cb_camera* camera);
void cb_camera_free(struct cb_camera* camera);

void cb_set_perspective(float fov, float aspect, float znear, float zfar);

#endif
