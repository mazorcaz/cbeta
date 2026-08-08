// camera.c

#include <cbeta/camera.h>

#include <stdbool.h>
#include <math.h>
#include <GL/gl.h>
#include <cbeta/world.h>
#include <cbeta/loc.h>
#include <cbeta/v3.h>
#include <cbeta/material.h>

void cb_camera_init(struct cb_camera* camera) {
	camera->x = 0.0f;
	camera->y = 0.0f;
	camera->z = 0.0f;
	
	camera->yaw = 0.0f;
	camera->pitch = 0.0f;

	camera->speed = 32.0f;
	camera->sensitivity = 0.3f;
}

void cb_camera_free(struct cb_camera* camera) {
	// do nothing
}

static void cb_camera_handle_mouse(struct cb_camera* camera, float dx, float dy) {
	camera->pitch += dy * camera->sensitivity;
	if (camera->pitch > 89.0f) camera->pitch = 89.0f;
	if (camera->pitch < -89.0f) camera->pitch = -89.0f;

	camera->yaw += dx * camera->sensitivity;
	if (camera->yaw >= 360.0f) camera->yaw -= 360.0f;
	if (camera->yaw < 0.0f) camera->yaw += 360.0f;
}

static void cb_camera_handle_keys(struct cb_camera* camera, const uint8_t* keys, uint64_t dt) {
	float rad_yaw = camera->yaw * (M_PI / 180.0f);

	float rate = camera->speed * ( ((float)dt ) / (float)SDL_GetPerformanceFrequency());
	
	float forward_x = sinf(rad_yaw) * rate;
	float forward_z = -cosf(rad_yaw) * rate;
	float right_x = cosf(rad_yaw) * rate;
	float right_z = sinf(rad_yaw) * rate;

	if (keys[SDL_SCANCODE_W]) { camera->x += forward_x; camera->z += forward_z; }
	if (keys[SDL_SCANCODE_S]) { camera->x -= forward_x; camera->z -= forward_z; }
	if (keys[SDL_SCANCODE_D]) { camera->x += right_x; camera->z += right_z; }
	if (keys[SDL_SCANCODE_A]) { camera->x -= right_x; camera->z -= right_z; }
	if (keys[SDL_SCANCODE_SPACE]) { camera->y += rate; }
	if (keys[SDL_SCANCODE_LSHIFT]) { camera->y -= rate; }
}

void cb_camera_handle(struct cb_camera* camera, SDL_Event* event, struct cb_world* world) {
	if (event->type == SDL_MOUSEMOTION) {
		cb_camera_handle_mouse(camera, event->motion.xrel, event->motion.yrel);
	} else if (event->type == SDL_MOUSEBUTTONDOWN) {
		
		float rad_yaw = camera->yaw * (M_PI / 180.0f);
		float rad_pitch = camera->pitch * (M_PI / 180.0f);
		float sin_yaw = sinf(rad_yaw);
		float cos_yaw = cosf(rad_yaw);
		float sin_pitch = sinf(rad_pitch);
		float cos_pitch = cosf(rad_pitch);
		
		struct cb_v3 dir = cb_v3(cos_pitch * sin_yaw, -sin_pitch, cos_pitch * -cos_yaw);
		struct cb_loc empty, solid;
		bool empty_found, solid_found;
		cb_world_raycast(
				world, cb_v3(camera->x, camera->y, camera->z), dir, 4.0f,
				&empty, &empty_found,
				&solid, &solid_found
		);
		
		if (event->button.button == SDL_BUTTON_LEFT) {
			if (solid_found) {
				cb_world_set_block(world, solid.x, solid.y, solid.z, CB_MATERIAL_AIR);
			}
		} else if (event->button.button == SDL_BUTTON_RIGHT) {
			if (empty_found) {
				cb_world_set_block(world, empty.x, empty.y, empty.z, CB_MATERIAL_STONE);
			}
		}
	}
}

void cb_camera_update(struct cb_camera* camera, uint64_t dt) {
	cb_camera_handle_keys(camera, SDL_GetKeyboardState(NULL), dt);
}

void cb_camera_apply(struct cb_camera* camera) {	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(camera->pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(camera->yaw, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camera->x, -camera->y, -camera->z);
}

void cb_set_perspective(float fov, float aspect, float znear, float zfar) {
	float f = (fov * (float)M_PI / 180.0f) / 2.0f;

	float top = znear * tanf(f);
	float bottom = -top;
	float right = top * aspect;
	float left = -right;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom, top, znear, zfar);
}
