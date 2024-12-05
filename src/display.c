#include "display.h"

SDL_Window* window = NULL; // this window is a pointer
SDL_Renderer* renderer = NULL; //Global Renderer pointer
uint32_t* color_buffer; //Global color buffer array pointer
SDL_Texture* color_buffer_texture = NULL; // Color Buffer texture pointer
int window_width = 800;
int window_height = 600;


bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Use SDL to query what the fullscreen max width and height are for machine
    SDL_DisplayMode display_mode; // declare struct and pass as reference param below
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    // Create a SDL Window
    window = SDL_CreateWindow(
        NULL, // title text (I think)
        SDL_WINDOWPOS_CENTERED, //x pos - center of window
        SDL_WINDOWPOS_CENTERED, //y pos - center
        window_width, // width
        window_height, // height
        SDL_WINDOW_BORDERLESS
    );
     // if pointer is null
    if (!window) {
        fprintf(stderr, "Error Creating SDL Window.\n");
        return false;
    }

    // Create a SDL Renderer
    // attach renderer with window
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); // true fullscreen

    return true;
}


void render_color_buffer(void) {
    // copy all content of color buffer to the texture
    SDL_UpdateTexture(
        color_buffer_texture,            // texture to update
        NULL,                            // rect - structure to represent area to update, NULL updates entire texture
        color_buffer,                    // pixels - raw pixel data - our color buffer
        (int)(window_width * sizeof(uint32_t)) // pitch - number of bytes in a row of pixel data, int
    );

    SDL_RenderCopy(
        renderer, // renderer
        color_buffer_texture, // texture
        NULL, // Null for entire texture
        NULL // Null for entire texture
    );  

}


void clear_color_buffer(uint32_t color) {
    // nested loop that goes through all rows and columns
    // and sets to color passed
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}


void draw_rect(int x, int y, int width, int height, uint32_t color) {
    // i -> go from 0 to width, j -> 0 to height and increment 
    // current x and y based off of starting x, y and fill in color in
    // these pixels
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int current_x = x + i;
            int current_y = y + j;
            color_buffer[(window_width * current_y) + current_x] = color;
        }
    }
}


void draw_grid(void) {
    // loop through rows and columns
    // for every pixel if x or y is multiple of 10, draw pixel gray
    // draws lines
    // for (int y = 0; y < window_height; y++) {
    //     for (int x = 0; x < window_width; x++) {
    //         if ( x % 10 == 0 || y % 10 == 0) {
    //             color_buffer[(window_width * y) + x] = 0xFF333333;
    //         }
    //     }
    // }
    // looping through ten pixels at a time - draws dot at this interval
    // instead of at every pixel if it's a multiple of 10 like above
    for (int y = 0; y < window_height; y += 10) {
        for (int x = 0; x < window_width; x += 10) {
            color_buffer[(window_width * y) + x] = 0xFF333333;
        }
    }
}


void draw_pixel(int x, int y, uint32_t color) {
    // if x and y are valid window positions - set draw pixel
    // color passed in at x,y coord
    if (x < window_width && y < window_height) {
        color_buffer[(window_width * y) + x] = color;
    }
}


void destroy_window(void) {
    // opposite of malloc - freeing memory allocation of color buffer
    // also destroy window and renderer - reverse order of initialize function
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}