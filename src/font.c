// font.c

#include <cbeta/font.h>

uint8_t cb_font_x[256];
uint8_t cb_font_y[256];
uint8_t cb_font_w[256];

void cb_font_bake() {
	cb_font_x[' '] = 0; cb_font_y[' '] = 0; cb_font_w[' '] = 4;
	cb_font_x['a'] = 1; cb_font_y['a'] = 6; cb_font_w['a'] = 6;
	cb_font_x['b'] = 2; cb_font_y['b'] = 6; cb_font_w['b'] = 6;
	cb_font_x['c'] = 3; cb_font_y['c'] = 6; cb_font_w['c'] = 6;
	cb_font_x['d'] = 4; cb_font_y['d'] = 6; cb_font_w['d'] = 6;
	cb_font_x['e'] = 5; cb_font_y['e'] = 6; cb_font_w['e'] = 6;
	cb_font_x['f'] = 6; cb_font_y['f'] = 6; cb_font_w['f'] = 5;
	cb_font_x['g'] = 7; cb_font_y['g'] = 6; cb_font_w['g'] = 6;
	cb_font_x['h'] = 8; cb_font_y['h'] = 6; cb_font_w['h'] = 6;
	cb_font_x['i'] = 9; cb_font_y['i'] = 6; cb_font_w['i'] = 2;
	cb_font_x['j'] = 10; cb_font_y['j'] = 6; cb_font_w['j'] = 6;
	cb_font_x['k'] = 11; cb_font_y['k'] = 6; cb_font_w['k'] = 5;
	cb_font_x['l'] = 12; cb_font_y['l'] = 6; cb_font_w['l'] = 3;
	cb_font_x['m'] = 13; cb_font_y['m'] = 6; cb_font_w['m'] = 6;
	cb_font_x['n'] = 14; cb_font_y['n'] = 6; cb_font_w['n'] = 6;
	cb_font_x['o'] = 15; cb_font_y['o'] = 6; cb_font_w['o'] = 6;
	cb_font_x['p'] = 0; cb_font_y['p'] = 7; cb_font_w['p'] = 6;
	cb_font_x['q'] = 1; cb_font_y['q'] = 7; cb_font_w['q'] = 6;
	cb_font_x['r'] = 2; cb_font_y['r'] = 7; cb_font_w['r'] = 6;
	cb_font_x['s'] = 3; cb_font_y['s'] = 7; cb_font_w['s'] = 6;
	cb_font_x['t'] = 4; cb_font_y['t'] = 7; cb_font_w['t'] = 4;
	cb_font_x['u'] = 5; cb_font_y['u'] = 7; cb_font_w['u'] = 6;
	cb_font_x['v'] = 6; cb_font_y['v'] = 7; cb_font_w['v'] = 6;
	cb_font_x['w'] = 7; cb_font_y['w'] = 7; cb_font_w['w'] = 6;
	cb_font_x['x'] = 8; cb_font_y['x'] = 7; cb_font_w['x'] = 6;
	cb_font_x['y'] = 9; cb_font_y['y'] = 7; cb_font_w['y'] = 6;
	cb_font_x['z'] = 10; cb_font_y['z'] = 7; cb_font_w['z'] = 6;
	
	cb_font_x['0'] = 0; cb_font_y['0'] = 3; cb_font_w['0'] = 6;
	cb_font_x['1'] = 1; cb_font_y['1'] = 3; cb_font_w['1'] = 6;
	cb_font_x['2'] = 2; cb_font_y['2'] = 3; cb_font_w['2'] = 6;
	cb_font_x['3'] = 3; cb_font_y['3'] = 3; cb_font_w['3'] = 6;
	cb_font_x['4'] = 4; cb_font_y['4'] = 3; cb_font_w['4'] = 6;
	cb_font_x['5'] = 5; cb_font_y['5'] = 3; cb_font_w['5'] = 6;
	cb_font_x['6'] = 6; cb_font_y['6'] = 3; cb_font_w['6'] = 5;
	cb_font_x['7'] = 7; cb_font_y['7'] = 3; cb_font_w['7'] = 6;
	cb_font_x['8'] = 8; cb_font_y['8'] = 3; cb_font_w['8'] = 6;
	cb_font_x['9'] = 9; cb_font_y['9'] = 3; cb_font_w['9'] = 6;
}

int cb_draw_char(char c, int x, int y, GLuint font) {
	if (c >= 32 && c <= 126) {
		float width = cb_font_w[c];
		
		float left = (float)cb_font_x[c] / 16.0f;
		float right = left + width / 128.0f;
		float top = (float)cb_font_y[c] / 16.0f;
		float bottom = top + 1/16.0f;
		
		glBindTexture(GL_TEXTURE_2D, font);
		glBegin(GL_QUADS);
		
		glColor3f(1.0f, 1.0f, 1.0f);
		
		glTexCoord2f(left, top);
		glVertex2f(x + 0.0f, y + 0.0f);
		
		glTexCoord2f(left, bottom);
		glVertex2f(x + 0.0f, y + 16.0f);
		
		glTexCoord2f(right, bottom);
		glVertex2f(x + width * 2, y + 16.0f);
		
		glTexCoord2f(right, top);
		glVertex2f(x + width * 2, y + 0.0f);
		
		glEnd();
		
		return cb_font_w[c] * 2;
	}
	
	return 0;
}

int cb_draw_string(const char* s, int x, int y, GLuint font) {
	int w = 0;
	while (*s) {
		w += cb_draw_char(*s++, x + w, y, font);
	}
	return w;
}
