/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/

/*
cbrain (brainfuck extension) interpreter with graphical capabilities. 
*/

//External includes
#include <stdio.h>
#include <stdint.h>
#include "../../include/SLK/SLK.h"
//-------------------------------------

//Internal includes
//-------------------------------------

//#defines
//2^24
#define MEM_SIZE 16777216
//How many instructions to run betwixt each frame
#define INSTR_PER_FRAME 4096
//-------------------------------------

//Typedefs
typedef enum 
{
   INSTR_NONE = 0,
   PTR_PLUS = 1, PTR_MINUS = 2,
   VAL_PLUS = 3, VAL_MINUS = 4,
   GET_VAL = 5, PUT_VAL = 6,
   WHILE_START = 7, WHILE_END = 8,
   PROC_START = 9, PROC_END = 10,
   PROC_CALL = 11,
}Instruction;

typedef struct Stack
{
   int ptr;
   struct Stack *next; 
}Stack;
//-------------------------------------

//Variables
static uint8_t mem[MEM_SIZE] = {0};
static uint8_t *ptr = mem;
static int instr_ptr = 0; //Not a real pointer
static Stack *stack = NULL;;
static Stack *stack_reserve = NULL;
static int nop = 0;
static SLK_Pal_sprite *frame;

//"Pointers" to all procedures
static int proc_table[256];

//Resizing buffer for instructions
static int instr_used = 0;
static int instr_space = 512;
static Instruction *instr = NULL;
//-------------------------------------

//Function prototypes
static void preprocess(FILE *in);
static inline void stack_push(int iptr);
static inline int stack_pull();
//-------------------------------------

//Function implementations

int main(int argc, char *argv[])
{
   int state = 0;
   int width = 320;
   int height = 240;
   char *path = NULL;
   char *path_pal = NULL;
   for(int i = 1;i<argc;i++)
   {
      if(argv[i][0]=='-')
      {
         switch(argv[i][1])
         {
         case 'h': puts("SoftLK-lib brainfuck example\nAvailible commandline options:\n\t-h\t\tprint this text\n\t-f [PATH]\tspecify the file to execute\n\t-x [SIZE]\tset the viewport width\n\t-y [SIZE\tset the viewport height\n\t-p [PATH]\tset the palette"); break;
         case 'f': state = 1; break;
         case 'x': state = 2; break;
         case 'y': state = 3; break;
         case 'p': state = 4; break;
         }
      }
      else
      {
         switch(state)
         {
         case 1: //File path
            path = argv[i];
            break;
         case 2: //Viewport width:
            width = atoi(argv[i]);
            break;
         case 3: //Viewport height:
            height = atoi(argv[i]);
            break;
         case 4: //Palette
            path_pal = argv[i];
            break;
         }
      }
   }
   if(path==NULL)
   {
      puts("Error: no input file specified, try ./brainfuck -h for help");
      exit(-1);
   }

   FILE *in = fopen(path,"r");
   preprocess(in);
   fclose(in);

   //Initialized memory
   for(int i = 0;i<256;i++)
      proc_table[i] = -1;
      
   SLK_setup(width,height,1,"SoftLK template",0,SLK_WINDOW_MAX,0); 
   SLK_timer_set_fps(30);

   SLK_layer_create(0,SLK_LAYER_PAL);
   SLK_layer_activate(0,1);
   SLK_layer_set_current(0);
   SLK_layer_set_dynamic(0,0);
   
   frame = SLK_pal_sprite_create(width,height);
   if(path_pal!=NULL)
      SLK_layer_set_palette(0,SLK_palette_load(path_pal));

   //Main loop.
   while(SLK_core_running())
   {
      SLK_update();

      for(int i = 0;i<INSTR_PER_FRAME&&instr_ptr<instr_used;i++)
      {
         if(nop)
         {
            int balance = 1;
            nop = 0;
            while(balance!=0) 
            {
               instr_ptr++;
               if(instr[instr_ptr]==WHILE_START) balance++;
               else if(instr[instr_ptr]==WHILE_END) balance--;
            }
            instr_ptr++;
         }

         switch(instr[instr_ptr])
         {
         case INSTR_NONE: instr_ptr++; break;
         case PTR_PLUS: ++ptr; instr_ptr++; break;
         case PTR_MINUS: --ptr; instr_ptr++; break;
         case VAL_PLUS: ++*ptr; instr_ptr++; break;
         case VAL_MINUS: --*ptr; instr_ptr++; break;
         case GET_VAL: *ptr = getchar();instr_ptr++; break;
         case PUT_VAL: putchar(*ptr); instr_ptr++; break;
         case WHILE_END: instr_ptr = stack_pull(); break;
         case PROC_END: instr_ptr = stack_pull(); break;
         case WHILE_START:
            stack_push(instr_ptr);
            instr_ptr++;
            if(!(*ptr))
            {
               instr_ptr = stack_pull();
               nop = 1;
            }
            break;
         case PROC_START:
            proc_table[*ptr] = instr_ptr+1;
            {
               int balance = 1;
               while(balance!=0) 
               {
                  instr_ptr++;
                  if(instr[instr_ptr]==PROC_START) balance++;
                  else if(instr[instr_ptr]==PROC_END) balance--;
               }
               instr_ptr++;
            }
            break;
         case PROC_CALL: 
            stack_push(instr_ptr+1);
            instr_ptr = proc_table[*ptr];
            break;
         }
      }
      fflush(stdout);

      SLK_layer_set_current(0);
      for(int i = 0;i<width*height;i++)
         frame->data[i].index = mem[i];
      SLK_draw_pal_sprite(frame,0,0);
         
      SLK_render_update();
   }

   for(int i = 0;i<256;i++)
      if(proc_table[i]!=-1)
         printf("procedure %d: %d\n",i,proc_table[i]);

   return 0;
}

static void preprocess(FILE *in)
{
   instr = malloc(sizeof(*instr)*instr_space);
   while(!feof(in))
   {
      char c = fgetc(in);
      Instruction next = 0;
      switch(c)
      {
      case '>': next = PTR_PLUS; break;
      case '<': next = PTR_MINUS; break;
      case '+': next = VAL_PLUS; break;
      case '-': next = VAL_MINUS; break;
      case ',': next = GET_VAL; break;
      case '.': next = PUT_VAL; break;
      case '[': next = WHILE_START; break;
      case ']': next = WHILE_END; break;
      case '(': next = PROC_START; break;
      case ')': next = PROC_END; break;
      case ':': next = PROC_CALL; break;
      }

      if(next!=0)
      {
         instr[instr_used] = next; 
         instr_used++;
         if(instr_used>=instr_space)
         {
            instr_space+=512;
            instr = realloc(instr,sizeof(*instr)*instr_space);
         }
      }
   }
}

static inline void stack_push(int iptr)
{
   if(stack_reserve==NULL)
   {
      Stack *s = malloc(sizeof(*s));
      s->ptr = iptr;
      s->next = stack;
      stack = s;

      return;
   }

   Stack *s = stack_reserve;
   stack_reserve = s->next;
   s->ptr = iptr;
   s->next = stack;
   stack = s;
}

static inline int stack_pull()
{
   Stack *s = stack;
   stack = s->next;
   s->next = stack_reserve;
   stack_reserve = s;

   return s->ptr;
}
//-------------------------------------
