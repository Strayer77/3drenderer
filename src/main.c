#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

//Global Variables
SDL_Window* window = NULL; // this window is a pointer
SDL_Renderer* renderer = NULL; //Global Renderer pointer

bool is_running = false;

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


void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw, render, clear - inside game loop
    draw_grid();
                                    
    draw_rect(300, 200, 300, 150, 0xFF598a5c);

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}


void destroy_window(void) {
    // opposite of malloc - freeing memory allocation of color buffer
    // also destroy window and renderer - reverse order of initialize function
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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