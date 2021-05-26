//Set the window title.
void backend_set_title(const char *title)
{
   SDL_SetWindowTitle(sdl_window,title);
}

//Toggles fullscreen.
void backend_set_fullscreen(int fullscreen)
{
   if(fullscreen)
   {
      SDL_SetWindowFullscreen(sdl_window,SDL_WINDOW_FULLSCREEN_DESKTOP);
   }
   else
   {
      SDL_SetWindowFullscreen(sdl_window,0);
      SDL_SetWindowSize(sdl_window,screen_width*pixel_scale,screen_height*pixel_scale);
   }

   backend_update_viewport();
}

//Sets wether the window is visible.
void backend_set_visible(int visible)
{
   if(visible)
      SDL_ShowWindow(sdl_window);
   else
      SDL_HideWindow(sdl_window);
}

//Sets the window icon.
void backend_set_icon(const SLK_RGB_sprite *icon)
{
   SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(icon->data,icon->width,icon->height,32,icon->width*4,0xf000,0x0f00,0x00f0,0x000f);

   SDL_SetWindowIcon(sdl_window,surface);

   SDL_FreeSurface(surface);
}

//Returns the viewport width adjusted to pixel scale.
int backend_get_width()
{
   return screen_width;
}

//Returns the viewport height adjusted to pixel scale.
int backend_get_height()
{
   return screen_height;
}

//Returns the view width.
int backend_get_view_width()
{
   return view_width;
}

//Returns the view height.
int backend_get_view_height()
{
   return view_height;
}

//Returns the view x pos.
int backend_get_view_x()
{
   return view_x;
}

//Returns the view y pos.
int backend_get_view_y()
{
   return view_y;
}

//Returns the window width.
int backend_get_win_width()
{
   return window_width;
}

//Returns the window height.
int backend_get_win_height()
{
   return window_height;
}

//Sets the target/maximum fps.
void backend_set_fps(int FPS)
{
   if(FPS<1||FPS>1000)
      fps = 1000;
   else
      fps = FPS;

   framedelay = 1000/fps;
}

//Returns the current target fps.
int backend_get_fps()
{
   return fps;
}

//Limits the fps to the target fps.
void backend_timer_update()
{
   frametime = SDL_GetTicks()-framestart;

   if(framedelay>frametime)
      SDL_Delay(framedelay-frametime);

   delta = (float)(SDL_GetTicks()-framestart)/1000.0f;
   framestart = SDL_GetTicks();
}

//Returns the delta time of the last frame.
float backend_timer_get_delta()
{
   return delta;
}

//Creates the keymap.
void backend_input_init()
{
   key_map[0x00] = SLK_KEY_NONE;
   key_map[SDL_SCANCODE_A] = SLK_KEY_A;
   key_map[SDL_SCANCODE_B] = SLK_KEY_B;
   key_map[SDL_SCANCODE_C] = SLK_KEY_C;
   key_map[SDL_SCANCODE_D] = SLK_KEY_D;
   key_map[SDL_SCANCODE_E] = SLK_KEY_E;
   key_map[SDL_SCANCODE_F] = SLK_KEY_F;
   key_map[SDL_SCANCODE_G] = SLK_KEY_G;
   key_map[SDL_SCANCODE_H] = SLK_KEY_H;
   key_map[SDL_SCANCODE_I] = SLK_KEY_I;
   key_map[SDL_SCANCODE_J] = SLK_KEY_J;
   key_map[SDL_SCANCODE_K] = SLK_KEY_K;
   key_map[SDL_SCANCODE_L] = SLK_KEY_L;
   key_map[SDL_SCANCODE_M] = SLK_KEY_M;
   key_map[SDL_SCANCODE_N] = SLK_KEY_N;
   key_map[SDL_SCANCODE_O] = SLK_KEY_O;
   key_map[SDL_SCANCODE_P] = SLK_KEY_P;
   key_map[SDL_SCANCODE_Q] = SLK_KEY_Q;
   key_map[SDL_SCANCODE_R] = SLK_KEY_R;
   key_map[SDL_SCANCODE_S] = SLK_KEY_S;
   key_map[SDL_SCANCODE_T] = SLK_KEY_T;
   key_map[SDL_SCANCODE_U] = SLK_KEY_U;
   key_map[SDL_SCANCODE_V] = SLK_KEY_V;
   key_map[SDL_SCANCODE_W] = SLK_KEY_W;
   key_map[SDL_SCANCODE_X] = SLK_KEY_X;
   key_map[SDL_SCANCODE_Y] = SLK_KEY_Y;
   key_map[SDL_SCANCODE_Z] = SLK_KEY_Z;

   key_map[SDL_SCANCODE_F1] = SLK_KEY_F1;
   key_map[SDL_SCANCODE_F2] = SLK_KEY_F2;
   key_map[SDL_SCANCODE_F3] = SLK_KEY_F3;
   key_map[SDL_SCANCODE_F4] = SLK_KEY_F4;
   key_map[SDL_SCANCODE_F5] = SLK_KEY_F5;
   key_map[SDL_SCANCODE_F6] = SLK_KEY_F6;
   key_map[SDL_SCANCODE_F7] = SLK_KEY_F7;
   key_map[SDL_SCANCODE_F8] = SLK_KEY_F8;
   key_map[SDL_SCANCODE_F9] = SLK_KEY_F9;
   key_map[SDL_SCANCODE_F10] = SLK_KEY_F10;
   key_map[SDL_SCANCODE_F11] = SLK_KEY_F11;
   key_map[SDL_SCANCODE_F12] = SLK_KEY_F12;

   key_map[SDL_SCANCODE_DOWN] = SLK_KEY_DOWN;
   key_map[SDL_SCANCODE_LEFT] = SLK_KEY_LEFT;
   key_map[SDL_SCANCODE_RIGHT] = SLK_KEY_RIGHT;
   key_map[SDL_SCANCODE_UP] = SLK_KEY_UP;
   key_map[SDL_SCANCODE_RETURN] = SLK_KEY_ENTER;

   key_map[SDL_SCANCODE_BACKSPACE] = SLK_KEY_BACK;
   key_map[SDL_SCANCODE_ESCAPE] = SLK_KEY_ESCAPE;
   key_map[SDL_SCANCODE_TAB] = SLK_KEY_TAB;
   key_map[SDL_SCANCODE_LGUI] = SLK_KEY_HOME;
   key_map[SDL_SCANCODE_END] = SLK_KEY_END;
   key_map[SDL_SCANCODE_PAGEUP] = SLK_KEY_PGUP;
   key_map[SDL_SCANCODE_PAGEDOWN] = SLK_KEY_PGDN;
   key_map[SDL_SCANCODE_INSERT] = SLK_KEY_INS;
   key_map[SDL_SCANCODE_LSHIFT] = SLK_KEY_SHIFT;
   key_map[SDL_SCANCODE_RSHIFT] = SLK_KEY_SHIFT;
   key_map[SDL_SCANCODE_LCTRL] = SLK_KEY_CTRL;
   key_map[SDL_SCANCODE_RCTRL] = SLK_KEY_CTRL;
   key_map[SDL_SCANCODE_SPACE] = SLK_KEY_SPACE;

   key_map[SDL_SCANCODE_0] = SLK_KEY_0;
   key_map[SDL_SCANCODE_1] = SLK_KEY_1;
   key_map[SDL_SCANCODE_2] = SLK_KEY_2;
   key_map[SDL_SCANCODE_3] = SLK_KEY_3;
   key_map[SDL_SCANCODE_4] = SLK_KEY_4;
   key_map[SDL_SCANCODE_5] = SLK_KEY_5;
   key_map[SDL_SCANCODE_6] = SLK_KEY_6;
   key_map[SDL_SCANCODE_7] = SLK_KEY_7;
   key_map[SDL_SCANCODE_8] = SLK_KEY_8;
   key_map[SDL_SCANCODE_9] = SLK_KEY_9;

   key_map[SDL_SCANCODE_KP_0] = SLK_KEY_NP0;
   key_map[SDL_SCANCODE_KP_1] = SLK_KEY_NP1;
   key_map[SDL_SCANCODE_KP_2] = SLK_KEY_NP2;
   key_map[SDL_SCANCODE_KP_3] = SLK_KEY_NP3;
   key_map[SDL_SCANCODE_KP_4] = SLK_KEY_NP4;
   key_map[SDL_SCANCODE_KP_5] = SLK_KEY_NP5;
   key_map[SDL_SCANCODE_KP_6] = SLK_KEY_NP6;
   key_map[SDL_SCANCODE_KP_7] = SLK_KEY_NP7;
   key_map[SDL_SCANCODE_KP_8] = SLK_KEY_NP8;
   key_map[SDL_SCANCODE_KP_9] = SLK_KEY_NP9;
   key_map[SDL_SCANCODE_KP_MULTIPLY] = SLK_KEY_NP_MUL;
   key_map[SDL_SCANCODE_KP_PLUS] = SLK_KEY_NP_ADD;
   key_map[SDL_SCANCODE_KP_DIVIDE] = SLK_KEY_NP_DIV;
   key_map[SDL_SCANCODE_KP_MINUS] = SLK_KEY_NP_SUB;
   key_map[SDL_SCANCODE_KP_PERIOD] = SLK_KEY_NP_DECIMAL;

   mouse_map[SDL_BUTTON_LEFT] = SLK_BUTTON_LEFT;
   mouse_map[SDL_BUTTON_RIGHT] = SLK_BUTTON_RIGHT;
   mouse_map[SDL_BUTTON_MIDDLE] = SLK_BUTTON_MIDDLE;
   mouse_map[SDL_BUTTON_X1] = SLK_BUTTON_X1;
   mouse_map[SDL_BUTTON_X2] = SLK_BUTTON_X2;

   gamepad_map[SDL_CONTROLLER_BUTTON_A] = SLK_PAD_A;
   gamepad_map[SDL_CONTROLLER_BUTTON_B] = SLK_PAD_B;
   gamepad_map[SDL_CONTROLLER_BUTTON_X] = SLK_PAD_X;
   gamepad_map[SDL_CONTROLLER_BUTTON_Y] = SLK_PAD_Y;
   gamepad_map[SDL_CONTROLLER_BUTTON_BACK] = SLK_PAD_BACK;
   gamepad_map[SDL_CONTROLLER_BUTTON_GUIDE] = SLK_PAD_GUIDE;
   gamepad_map[SDL_CONTROLLER_BUTTON_START] = SLK_PAD_START;
   gamepad_map[SDL_CONTROLLER_BUTTON_LEFTSTICK] = SLK_PAD_LEFTSTICK;
   gamepad_map[SDL_CONTROLLER_BUTTON_RIGHTSTICK] = SLK_PAD_RIGHTSTICK;
   gamepad_map[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = SLK_PAD_LEFTSHOULDER;
   gamepad_map[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = SLK_PAD_RIGHTSHOULDER;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_UP] = SLK_PAD_UP;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = SLK_PAD_DOWN;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = SLK_PAD_LEFT;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = SLK_PAD_RIGHT;

   //Clear key states, just in case,
   //should already be empty since known at compile time
   memset(new_key_state,0,sizeof(new_key_state));
   memset(old_key_state,0,sizeof(old_key_state));
   memset(new_mouse_state,0,sizeof(new_mouse_state));
   memset(old_mouse_state,0,sizeof(old_mouse_state));
   for(int i = 0;i<MAX_CONTROLLERS;i++)
   {
      memset(gamepads[i].new_button_state,0,sizeof(gamepads[i].new_button_state));
      memset(gamepads[i].old_button_state,0,sizeof(gamepads[i].old_button_state));
   }
}

//Shows or hides the mouse cursor.
void backend_show_cursor(int shown)
{
   SDL_ShowCursor(shown?SDL_ENABLE:SDL_DISABLE);
}

//Sets wether the mouse cursor is captured and only relative
//mouse motion is registerd.
void backend_mouse_set_relative(int relative)
{
   SDL_SetRelativeMouseMode(relative);
}

//Sets wether to track mouse events globally.
void backend_mouse_capture(int capture)
{
   SDL_CaptureMouse(capture);
}

//Starts text input.
void backend_start_text_input(char *text)
{
   text_input = text;
   text_input_active = 1;

   SDL_StartTextInput();
}

//Stops text input.
void backend_stop_text_input()
{
   text_input_active = 0;
   
   SDL_StopTextInput();
}

int backend_key_down(int key)
{
   return new_key_state[key];
}

int backend_key_pressed(int key)
{
   return new_key_state[key]&&!old_key_state[key];
}

int backend_key_released(int key)
{
   return !new_key_state[key]&&old_key_state[key];
}

SLK_Button backend_key_get_state(int key)
{
   SLK_Button out;
   out.pressed = new_key_state[key]&&!old_key_state[key];
   out.released = !new_key_state[key]&&old_key_state[key];
   out.held = new_key_state[key];

   return out;
}

int backend_mouse_down(int key)
{
   return new_mouse_state[key];
}

int backend_mouse_pressed(int key)
{
   return new_mouse_state[key]&&!old_mouse_state[key];
}

int backend_mouse_released(int key)
{
   return !new_mouse_state[key]&&old_mouse_state[key];
}

SLK_Button backend_mouse_get_state(int key)
{
   SLK_Button out;
   out.pressed = new_mouse_state[key]&&!old_mouse_state[key];
   out.held = new_mouse_state[key];
   out.released = !new_mouse_state[key]&&old_mouse_state[key];

   return out;
}

int backend_mouse_wheel_get_scroll()
{
   return mouse_wheel;
}

int backend_gamepad_down(int index, int key)
{
   return gamepads[index].new_button_state[key];
}

int backend_gamepad_pressed(int index, int key)
{
   return gamepads[index].new_button_state[key]&&!gamepads[index].old_button_state[key];
}

int backend_gamepad_released(int index, int key)
{
   return !gamepads[index].new_button_state[key]&&gamepads[index].old_button_state[key];
}

SLK_Button backend_gamepad_get_state(int index, int key)
{
   SLK_Button out;
   out.pressed = gamepads[index].new_button_state[key]&&!gamepads[index].old_button_state[key];
   out.held = gamepads[index].new_button_state[key];
   out.released = !gamepads[index].new_button_state[key]&&gamepads[index].old_button_state[key];

   return out;
}

int backend_get_gamepad_count()
{
   return SDL_NumJoysticks();
}

void backend_mouse_get_pos(int *x, int *y)
{
   *x = mouse_x;
   *y = mouse_y;
}

void backend_mouse_get_relative_pos(int *x, int *y)
{
   *x = mouse_x_rel;
   *y = mouse_y_rel;
}

static int get_gamepad_index(int which)
{

   for(int i = 0;i<MAX_CONTROLLERS;i++)
      if(gamepads[i].connected&&gamepads[i].id==which)
         return i;

   return -1;
}

#if SLK_ENABLE_RGB

SLK_RGB_sprite *backend_load_rgb(const char *path)
{
   cp_image_t img = cp_load_png(path);
   SLK_RGB_sprite *out = NULL;
   if(img.pix==0)
   {
      printf("Failed to load %s\n",path);
      return NULL;
   }

   out = SLK_rgb_sprite_create(img.w,img.h);
   memcpy(out->data,img.pix,img.w*img.h*sizeof(*out->data));
   cp_free_png(&img);

   return out;
}

SLK_RGB_sprite *backend_load_rgb_file(FILE *f)
{
   int size = 0;
   fseek(f,0,SEEK_END);
   size = ftell(f);
   fseek(f,0,SEEK_SET);
   char *data = malloc(size+1);
   fread(data,size,1,f);
   data[size] = 0;
   SLK_RGB_sprite *out = backend_load_rgb_mem(data,size);
   free(data);

   return out;
}

SLK_RGB_sprite *backend_load_rgb_mem(const void *data, int length)
{
   cp_image_t img = cp_load_png_mem(data,length);
   SLK_RGB_sprite *out = NULL;
   if(img.pix==0)
   {
      puts("Failed to load png from mem");
      return NULL;
   }

   out = SLK_rgb_sprite_create(img.w,img.h);
   memcpy(out->data,img.pix,img.w*img.h*sizeof(*out->data));
   cp_free_png(&img);

   return out;
}


void backend_save_rgb(const SLK_RGB_sprite *s, const char *path)
{
   if(!path)
      return;
   FILE *f = fopen(path,"wb");

   backend_save_rgb_file(s,f);

   fclose(f);
}

void backend_save_rgb_file(const SLK_RGB_sprite *s, FILE *f)
{
   if(!f)
      return;
   cp_image_t img;
   img.w = s->width;
   img.h = s->height;
   img.pix = (cp_pixel_t *)s->data;
   cp_save_png(f,&img);
}

#endif

#if SLK_ENABLE_PAL

SLK_Pal_sprite *backend_load_pal(const char *path)
{
   FILE *f = fopen(path,"rb");
   if(f==NULL)
   {
      printf("Failed to load %s\n",path);
      return NULL;
   }
   SLK_Pal_sprite *out = backend_load_pal_file(f);
   fclose(f);
   return out;
}

SLK_Pal_sprite *backend_load_pal_file(FILE *f)
{
   int size = 0;
   char *data = NULL;
   fseek(f,0,SEEK_END);
   size = ftell(f);
   fseek(f,0,SEEK_SET);
   data = malloc(size+1);
   fread(data,size,1,f);
   data[size] = 0;
   SLK_Pal_sprite *s = backend_load_pal_mem(data,size);
   free(data);

   return s;
}

SLK_Pal_sprite *backend_load_pal_mem(const void *data, int length)
{
   ULK_slk_image *img = ULK_slk_image_load_mem_buffer(data,length);
   SLK_Pal_sprite *s = SLK_pal_sprite_create(img->width,img->height);
   memcpy(s->data,img->data,sizeof(*s->data)*s->width*s->height);
   ULK_slk_image_free(img);

   return s;
}

void backend_save_pal(const SLK_Pal_sprite *s, const char *path, int rle)
{
   FILE *f = fopen(path,"wb");

   if(!f)
      return;

   backend_save_pal_file(s,f,rle);
      
   fclose(f);
}

void backend_save_pal_file(const SLK_Pal_sprite *s, FILE *f, int rle)
{
   ULK_slk_image img;
   img.width = s->width;
   img.height = s->height;
   img.data = (uint8_t *)s->data;
   ULK_slk_image_write(&img,f,rle);
}

#endif

SLK_Palette *backend_load_palette(const char *path)
{
   FILE *f = fopen(path,"r");
   SLK_Palette *palette = backend_load_palette_file(f);
   fclose(f);
   return palette;
}

void backend_save_palette(const char *path, const SLK_Palette *pal)
{
   FILE *f = fopen(path,"w");
   backend_save_palette_file(f,pal);
  fclose(f);
}

SLK_Palette *backend_load_palette_file(FILE *f)
{
   if(!f)
      return NULL;

   char buffer[512];
   int colors = 0,i,found;
   int r,g,b,a;

   SLK_Palette *palette = malloc(sizeof(*palette));
   memset(palette,0,sizeof(*palette));
   for(i = 0;i<259&&fgets(buffer,512,f);i++)
   {
      if(i==2)
      {
         sscanf(buffer,"%d",&found);
         palette->used = found;
      }
      else if(i>2&&buffer[0]!='\0')
      {
         if(sscanf(buffer,"%d %d %d %d",&r,&g,&b,&a)!=4)
         {
            sscanf(buffer,"%d %d %d",&r,&g,&b);
            a = 255;
         }

         palette->colors[colors].r = r;
         palette->colors[colors].g = g;
         palette->colors[colors].b = b;
         palette->colors[colors].a = a;
         colors++;
      }
   }

   return palette;
}

void backend_save_palette_file(FILE *f, const SLK_Palette *pal)
{
   if(!f)
      return;
   fprintf(f,"JASC-PAL\n0100\n%d\n",pal->used);
   for(int i = 0;i<pal->used;i++)
   {
      if(pal->colors[i].a!=255)
         fprintf(f,"%d %d %d %d\n",pal->colors[i].r,pal->colors[i].g,pal->colors[i].b,pal->colors[i].a);
      else
         fprintf(f,"%d %d %d\n",pal->colors[i].r,pal->colors[i].g,pal->colors[i].b);
   }
}
