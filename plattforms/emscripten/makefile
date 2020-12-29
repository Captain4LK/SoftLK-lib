CC = emcc
CFLAGS=-lm -Wall -c
DEPENDENCIES  = SLK_core.o SLK_color.o SLK_draw_pal.o SLK_draw_rgb.o SLK_input.o SLK_layer.o SLK_palette.o SLK_sprite_pal.o SLK_sprite_rgb.o SLK_timer.o SLK_sheet_rgb.o SLK_sheet_pal.o ULK_slk.o

.PHONY: backend_emscripten
.PHONY: SLK_3d_fixed
.PHONY: install

backend_emscripten: $(DEPENDENCIES) backend_emscripten_sdl2.o
	emar crs libSLK.a $^

SLK_3d_fixed: ULK_vector.o ULK_matrix.o ULK_3d.o SLK_3d.o SLK_3d_rgb.o kixor_list.o kixor_obj_parser.o kixor_string_extra.o
	emar crs "libSLK3d_fixed.a" $^

SLK_core.o: ../../src/SLK_core.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h ../../src/backend.h ../../src/SLK_layer_i.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_color.o: ../../src/SLK_color.c ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_draw_pal.o: ../../src/SLK_draw_pal.c ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h ../../src/SLK_draw_pal_i.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_draw_rgb.o: ../../src/SLK_draw_rgb.c ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h ../../src/SLK_draw_rgb_i.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_input.o: ../../src/SLK_input.c  ../../include/SLK/SLK_types.h ../../src/backend.h ../../src/SLK_layer_i.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_layer.o: ../../src/SLK_layer.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h ../../src/backend.h ../../src/SLK_draw_rgb_i.h ../../src/SLK_draw_pal_i.h ../../src/SLK_layer_i.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_palette.o: ../../src/SLK_palette.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_sprite_pal.o: ../../src/SLK_sprite_pal.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_sprite_rgb.o: ../../src/SLK_sprite_rgb.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_timer.o: ../../src/SLK_timer.c ../../include/SLK/SLK_functions.h ../../src/backend.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_sheet_rgb.o: ../../src/SLK_sheet_rgb.c ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_sheet_pal.o: ../../src/SLK_sheet_pal.c ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h
	$(CC) -O2 -c $< $(CFLAGS) 

ULK_slk.o: ../../external/UtilityLK/src/ULK_slk.c ../../external/UtilityLK/include/ULK_slk.h
	$(CC) -O2 -c $< $(CFLAGS) 

backend_emscripten_sdl2.o: ./backend_emscripten_sdl2.c ../../src/backend.h ../../external/UtilityLK/include/ULK_slk.h
	$(CC) -O2 -c $< -lm -Wall -s USE_SDL=2 -s USE_LIBPNG=1

ULK_vector.o: ../../external/UtilityLK/src/ULK_vector.c ../../external/UtilityLK/include/ULK_vector.h
	$(CC) -O2 -c $< $(CFLAGS) 

ULK_matrix.o: ../../external/UtilityLK/src/ULK_matrix.c ../../external/UtilityLK/include/ULK_matrix.h
	$(CC) -O2 -c $< $(CFLAGS) 

ULK_3d.o: ../../external/UtilityLK/src/ULK_3d.c ../../external/UtilityLK/include/ULK_3d.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_3d.o: ../../extensions/3d_renderer/SLK_3d.c ../../include/SLK/SLK_3d.h ../../extensions/3d_renderer/SLK_3d_config.h ../../external/UtilityLK/include/ULK_vector.h ../../external/UtilityLK/include/ULK_matrix.h ../../external/UtilityLK/include/ULK_3d.h
	$(CC) -O2 -c $< $(CFLAGS) 

SLK_3d_rgb.o: ../../extensions/3d_renderer/SLK_3d_rgb.c ../../include/SLK/SLK_3d.h ../../extensions/3d_renderer/SLK_3d_config.h ../../external/UtilityLK/include/ULK_vector.h ../../external/UtilityLK/include/ULK_matrix.h ../../external/UtilityLK/include/ULK_3d.h
	$(CC) -O2 -c $< $(CFLAGS) 

kixor_list.o: ../../extensions/3d_renderer/kixor/kixor_list.c
	$(CC) -O2 -c $< $(CFLAGS) 

kixor_obj_parser.o: ../../extensions/3d_renderer/kixor/kixor_obj_parser.c
	$(CC) -O2 -c $< $(CFLAGS) 

kixor_string_extra.o: ../../extensions/3d_renderer/kixor/kixor_string_extra.c
	$(CC) -O2 -c $< $(CFLAGS) 

clean:
	rm -f *.o *.a
