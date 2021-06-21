CC = emcc
CFLAGS=-lm -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -flto -c -s ASYNCIFY
DEPENDENCIES  = SLK_core.o SLK_color.o SLK_draw_pal.o SLK_draw_rgb.o SLK_input.o SLK_layer.o SLK_palette.o SLK_sprite_pal.o SLK_sprite_rgb.o SLK_timer.o ULK_slk.o SLK_gui.o
HEADERS = ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h ../../include/SLK/SLK_config.h

.PHONY: backend_emscripten
.PHONY: install

backend_emscripten: $(DEPENDENCIES) backend_emscripten_sdl2.o
	emar crs libSLK.a $^

SLK_core.o: ../../src/SLK_core.c ../../src/backend.h ../../src/SLK_layer_i.h $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_color.o: ../../src/SLK_color.c $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_draw_pal.o: ../../src/SLK_draw_pal.c ../../src/SLK_draw_pal_i.h $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS)

SLK_draw_rgb.o: ../../src/SLK_draw_rgb.c ../../src/SLK_draw_rgb_i.h $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_input.o: ../../src/SLK_input.c ../../src/backend.h ../../src/SLK_layer_i.h $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_layer.o: ../../src/SLK_layer.c ../../src/backend.h ../../src/SLK_draw_rgb_i.h ../../src/SLK_draw_pal_i.h ../../src/SLK_layer_i.h $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_palette.o: ../../src/SLK_palette.c $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_sprite_pal.o: ../../src/SLK_sprite_pal.c $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_sprite_rgb.o: ../../src/SLK_sprite_rgb.c $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_timer.o: ../../src/SLK_timer.c ../../src/backend.h $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_gui.o: ../../src/SLK_gui.c ../../include/SLK/SLK_gui.h $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

ULK_slk.o: ../../external/UtilityLK/src/ULK_slk.c ../../external/UtilityLK/include/ULK_slk.h $(HEADERS)
	$(CC) -O3 -c $< $(CFLAGS) 

backend_emscripten_sdl2.o: ./backend_emscripten_sdl2.c ../../src/backend.h ../../external/UtilityLK/include/ULK_slk.h
	$(CC) -O3 -c $< -lm -Wall -s USE_SDL=2 -s ASYNCIFY

clean:
	rm -f *.o *.a
