#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

//Global Variables
bool is_running = false;


void setup(void) {
    //allocate memory size
    // get number of bytes
    // multiply window height and width by size of color (uint32_t) in array
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height );

    // Create a SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888, // pixel format
        SDL_TEXTUREACCESS_STREAMING,// texture access
        window_width, // texture width
        window_height // texture height
    );
}


void process_input(void) {
    // read events - mainly from keyboard
    SDL_Event event; // create new event
    SDL_PollEvent(&event); //pass event

    switch (event.type) {
        case SDL_QUIT: // SDL_QUIT event type is when 'x' button is closed on window
           is_running = false;
           break; 
        case SDL_KEYDOWN: // check if key pressed
            if (event.key.keysym.sym == SDLK_ESCAPE) // if key pressed is 'escape'
                is_running = false;
            break;
    }
}


void update(void) {

}


void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw, render, clear - inside game loop
    draw_grid();

    draw_pixel(20, 20, 0xFFFFFF00);                    
    draw_rect(300, 200, 300, 150, 0xFF598a5c);

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}


int main(void) {

    is_running = initialize_window(); // true or false

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}