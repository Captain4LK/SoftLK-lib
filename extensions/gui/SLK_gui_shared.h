/*
SLK_gui - gui extension for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

This work's goal is to never be encumbered by any exclusive intellectual
property rights. The work is therefore provided under CC0 1.0 + additional
WAIVER OF ALL INTELLECTUAL PROPERTY RIGHTS that waives the rest of
intellectual property rights not already waived by CC0 1.0.
The CC0 license text and ip waiver can be found in the LICENSE file.
*/

#ifndef _SLK_GUI_SHARED_H_

#define _SLK_GUI_SHARED_H_

extern SLK_RGB_sprite *slk_gui_font;
extern SLK_Color slk_gui_color_0;
extern SLK_Color slk_gui_color_1;
extern SLK_Color slk_gui_color_2;
extern SLK_Color slk_gui_color_3;
extern SLK_Color slk_gui_color_4;
extern SLK_Color slk_gui_color_5;

int clip_text(char *dst, const char *src, int dst_size, SLK_gui_rectangle area);

#endif
