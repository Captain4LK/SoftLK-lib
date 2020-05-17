#ifndef _SLK_RENDER_I_H_

#define _SLK_RENDER_I_H_

#include <SDL2/SDL.h>

SDL_Window *sdl_window;
SDL_GLContext sdl_gl_context;
int screen_width;
int screen_height;
int pixel_scale;
int window_width;
int window_height;
int view_x;
int view_y;
int view_width;
int view_height;

void SLK_i_render_init();
void SLK_i_render_update_viewport();

#endif
