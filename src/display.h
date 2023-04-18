#pragma once
void display_init();
void display_clear(bool black = false);
void display_render();
void display_fill_rect(int x, int y, int w, int h, bool black = false);
void display_draw_line(int x0, int y0, int x1, int y1, bool black = false);
void display_draw_rect(int x, int y, int w, int h, bool black = false);
void display_draw_cross(int x, int y, int w, int h, bool black = false);