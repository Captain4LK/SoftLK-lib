CC = emcc -O3 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -flto
DEPENDENCIES  = SLK_core.o SLK_color.o SLK_draw_pal.o SLK_draw_rgb.o SLK_input.o SLK_layer.o SLK_palette.o SLK_sprite_pal.o SLK_sprite_rgb.o SLK_timer.o SLK_gui.o SLK_error.o
HEADERS = ../../include/SLK/SLK_functions.h ../../include/SLK/SLK_types.h ../../include/SLK/SLK_config.h ../../src/backend.h

.PHONY: backend_emscripten
.PHONY: install

backend_sdl2: $(DEPENDENCIES) backend_sdl2.o
	emar crs libSLK.a $^

SLK_core.o: ../../src/SLK_core.c ../../src/SLK_layer_i.h $(HEADERS)
	$(CC) -c $<

SLK_error.o: ../../src/SLK_error.c $(HEADERS)
	$(CC) -c $<

SLK_color.o: ../../src/SLK_color.c $(HEADERS)
	$(CC) -c $<

SLK_draw_pal.o: ../../src/SLK_draw_pal.c ../../src/SLK_draw_pal_i.h $(HEADERS)
	$(CC) -c $<

SLK_draw_rgb.o: ../../src/SLK_draw_rgb.c ../../src/SLK_draw_rgb_i.h $(HEADERS)
	$(CC) -c $<

SLK_input.o: ../../src/SLK_input.c ../../src/SLK_layer_i.h $(HEADERS)
	$(CC) -c $<

SLK_layer.o: ../../src/SLK_layer.c ../../src/SLK_draw_rgb_i.h ../../src/SLK_draw_pal_i.h ../../src/SLK_layer_i.h $(HEADERS)
	$(CC) -c $<

SLK_palette.o: ../../src/SLK_palette.c $(HEADERS)
	$(CC) -c $<

SLK_sprite_pal.o: ../../src/SLK_sprite_pal.c $(HEADERS)
	$(CC) -c $<

SLK_sprite_rgb.o: ../../src/SLK_sprite_rgb.c $(HEADERS)
	$(CC) -c $<

SLK_timer.o: ../../src/SLK_timer.c $(HEADERS)
	$(CC) -c $<

SLK_gui.o: ../../src/SLK_gui.c ../../include/SLK/SLK_gui.h $(HEADERS)
	$(CC) -c $<

# Doesn't like -flto for some reason...
backend_sdl2.o: ../../src/backend/backend_sdl2.c $(HEADERS)
	emcc -c $< -O3 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter

clean:
	rm -f *.o *.a
