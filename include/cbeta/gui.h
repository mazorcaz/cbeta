// gui.h

#ifndef CBETA_GUI_H
#define CBETA_GUI_H

#include <stdbool.h>

struct cb_gui {
	struct cb_resource* font;
};

bool cb_gui_init(struct cb_gui* gui);
void cb_gui_free(struct cb_gui* gui);
void cb_gui_render(struct cb_gui* gui);

#endif
