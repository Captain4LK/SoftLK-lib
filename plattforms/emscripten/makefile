CC = emcc
CFLAGS=-lm -Wall -c -s ASYNCIFY
DEPENDENCIES  = SLK_core.o SLK_color.o SLK_draw_pal.o SLK_draw_rgb.o SLK_input.o SLK_layer.o SLK_palette.o SLK_sprite_pal.o SLK_sprite_rgb.o SLK_timer.o ULK_slk.o

.PHONY: backend_emscripten
.PHONY: install

backend_emscripten: $(DEPENDENCIES) backend_emscripten_sdl2.o
	emar crs libSLK.a $^

SLK_core.o: ../../src/SLK_core.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h ../../src/backend.h ../../src/SLK_layer_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_color.o: ../../src/SLK_color.c ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_draw_pal.o: ../../src/SLK_draw_pal.c ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h ../../src/SLK_draw_pal_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_draw_rgb.o: ../../src/SLK_draw_rgb.c ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h ../../src/SLK_draw_rgb_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_input.o: ../../src/SLK_input.c  ../../include/SLK/SLK_types.h ../../src/backend.h ../../src/SLK_layer_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_layer.o: ../../src/SLK_layer.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h ../../src/backend.h ../../src/SLK_draw_rgb_i.h ../../src/SLK_draw_pal_i.h ../../src/SLK_layer_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_palette.o: ../../src/SLK_palette.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_sprite_pal.o: ../../src/SLK_sprite_pal.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_sprite_rgb.o: ../../src/SLK_sprite_rgb.c ../../include/SLK/SLK_types.h ../../include/SLK/SLK_functions.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_timer.o: ../../src/SLK_timer.c ../../include/SLK/SLK_functions.h ../../src/backend.h
	$(CC) -O3 -c $< $(CFLAGS) 

ULK_slk.o: ../../external/UtilityLK/src/ULK_slk.c ../../external/UtilityLK/include/ULK_slk.h
	$(CC) -O3 -c $< $(CFLAGS) 

backend_emscripten_sdl2.o: ./backend_emscripten_sdl2.c ../../src/backend.h ../../external/UtilityLK/include/ULK_slk.h
	$(CC) -O3 -c $< -lm -Wall -s USE_SDL=2 -s ASYNCIFY

clean:
	rm -f *.o *.a
