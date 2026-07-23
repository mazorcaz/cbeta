// font.h

#ifndef CBETA_FONT_H
#define CBETA_FONT_H

#include <GL/gl.h>

extern uint8_t cb_font_x[];
extern uint8_t cb_font_y[];
extern uint8_t cb_font_w[];
void cb_font_bake();

int cb_draw_char(char c, int x, int y, GLuint font);
int cb_draw_string(const char* s, int x, int y, GLuint font);

#endif
