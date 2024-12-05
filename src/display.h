#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>

extern SDL_Window* window; // this window is a pointer
extern SDL_Renderer* renderer; //Global Renderer pointer
extern uint32_t* color_buffer; //Global color buffer array pointer
extern SDL_Texture* color_buffer_texture; // Color Buffer texture pointer
extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_grid(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void destroy_window(void);

#endif