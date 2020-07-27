#ifndef _SLK_INPUT_I_H_

#define _SLK_INPUT_I_H_

#include <stdint.h>

uint8_t new_key_state[256];
uint8_t old_key_state[256];
uint8_t new_mouse_state[6];
uint8_t old_mouse_state[6];
int mouse_x_rel;
int mouse_y_rel;
char *text_input;
int text_input_active;

void SLK_i_mouse_update(int x, int y);
void SLK_i_mouse_update_wheel(int wheel);
void SLK_i_input_init();

#endif
