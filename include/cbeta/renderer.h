// renderer.h

#ifndef CBETA_RENDERER_H
#define CBETA_RENDERER_H

#include <stdint.h>
#include <stdbool.h>
#include <cbeta/camera.h>
#include <cbeta/resource.h>
#include <cbeta/geometry.h>
#include <cbeta/segment.h>

struct cb_renderer {
	struct cb_resource terrain;
	struct cb_mesh mesh;
	struct cb_segment* segments;
};

bool cb_renderer_init(struct cb_renderer* renderer);
void cb_renderer_free(struct cb_renderer* renderer);
void cb_renderer_render(struct cb_renderer* renderer, struct cb_camera* camera);

#endif
