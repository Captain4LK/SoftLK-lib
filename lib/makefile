CC = gcc
CFLAGS+=-lm -lSDL2 -lGL -Wall
//CFLAGS+=-lm -lSDL2 -lGL -Wall -pg

libSLK.a: glad.o SLK_core.o SLK_color.o SLK_draw_pal.o SLK_draw_rgb.o SLK_input.o SLK_layer.o SLK_palette.o SLK_render.o SLK_sprite_pal.o SLK_sprite_rgb.o SLK_timer.o SLK_sheet_rgb.o SLK_sheet_pal.o
	ar cr libSLK.a $^

glad.o: ../src/glad/glad.c ../include/glad/glad.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_core.o: ../src/SLK/SLK_core.c ../include/SLK/SLK_types.h ../include/SLK/SLK_functions.h ../src/SLK/SLK_input_i.h ../src/SLK/SLK_render_i.h ../src/SLK/SLK_draw_rgb_i.h ../src/SLK/SLK_draw_pal_i.h ../src/SLK/SLK_layer_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_color.o: ../src/SLK/SLK_color.c ../include/SLK/SLK_functions.h ../include/SLK/SLK_types.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_draw_pal.o: ../src/SLK/SLK_draw_pal.c ../include/SLK/SLK_functions.h ../include/SLK/SLK_types.h ../src/SLK/SLK_draw_pal_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_draw_rgb.o: ../src/SLK/SLK_draw_rgb.c ../include/SLK/SLK_functions.h ../include/SLK/SLK_types.h ../src/SLK/SLK_draw_rgb_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_input.o: ../src/SLK/SLK_input.c  ../include/SLK/SLK_types.h ../src/SLK/SLK_input_i.h ../src/SLK/SLK_render_i.h ../src/SLK/SLK_layer_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_layer.o: ../src/SLK/SLK_layer.c ../include/glad/glad.h ../include/SLK/SLK_types.h ../include/SLK/SLK_functions.h ../src/SLK/SLK_render_i.h ../src/SLK/SLK_draw_rgb_i.h ../src/SLK/SLK_draw_pal_i.h ../src/SLK/SLK_layer_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_palette.o: ../src/SLK/SLK_palette.c ../include/SLK/SLK_types.h ../include/SLK/SLK_functions.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_render.o: ../src/SLK/SLK_render.c ../include/glad/glad.h ../include/SLK/SLK_types.h ../include/SLK/SLK_functions.h ../src/SLK/SLK_render_i.h ../src/SLK/SLK_layer_i.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_sprite_pal.o: ../src/SLK/SLK_sprite_pal.c ../include/SLK/SLK_types.h ../include/SLK/SLK_functions.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_sprite_rgb.o: ../src/SLK/SLK_sprite_rgb.c ../include/SLK/SLK_types.h ../include/SLK/SLK_functions.h 
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_timer.o: ../src/SLK/SLK_timer.c ../include/SLK/SLK_functions.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_sheet_rgb.o: ../src/SLK/SLK_sheet_rgb.c ../include/SLK/SLK_functions.h ../include/SLK/SLK_types.h
	$(CC) -O3 -c $< $(CFLAGS) 

SLK_sheet_pal.o: ../src/SLK/SLK_sheet_pal.c ../include/SLK/SLK_functions.h ../include/SLK/SLK_types.h
	$(CC) -O3 -c $< $(CFLAGS) 

clean:
	rm -f *.o *.a
