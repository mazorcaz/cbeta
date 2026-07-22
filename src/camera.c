// camera.c

#include <cbeta/camera.h>

void cb_camera_init(struct cb_camera* camera) {
	camera->x = 0.0f;
	camera->y = 0.0f;
	camera->z = 0.0f;
	
	camera->yaw = -90.0f;
	camera->pitch = 0.0f;

	camera->speed = 2.0f;
	camera->sensitivity = 0.5f;
}

void cb_camera_handle_mouse(struct cb_camera* camera, float dx, float dy) {
	camera->pitch += dy * camera->sensitivity;
	camera->yaw += dx * camera->sensitivity;
}

void cb_camera_handle_keys(struct cb_camera* camera, const uint8_t* keys, uint64_t dt) {
	float rad_yaw = camera->yaw * (M_PI / 180.0f);

	float rate = camera->speed * ( ((float)dt ) / 1000);
	
	float forward_x = cosf(rad_yaw) * rate;
	float forward_z = sinf(rad_yaw) * rate;
	float right_x = -sinf(rad_yaw) * rate;
	float right_z = cosf(rad_yaw) * rate;

	if (keys[SDL_SCANCODE_W]) { camera->x += forward_x; camera->z += forward_z; }
	if (keys[SDL_SCANCODE_S]) { camera->x -= forward_x; camera->z -= forward_z; }
	if (keys[SDL_SCANCODE_D]) { camera->x += right_x; camera->z += right_z; }
	if (keys[SDL_SCANCODE_A]) { camera->x -= right_x; camera->z -= right_z; }
	if (keys[SDL_SCANCODE_SPACE]) { camera->y += rate; }
	if (keys[SDL_SCANCODE_LSHIFT]) { camera->y -= rate; }
}

void cb_camera_free(struct cb_camera* camera) {
	// do nothing
}
