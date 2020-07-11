#include <SDL.h>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[])
{
    std::string app_title = "SDL_Guide 01: Basic Application";
    SDL_Event event = { 0 };
    bool should_quit = false;

    // INITIALIZATION:
    // -----------------------------------------------------------------------------------------------------------------
    // Initialize SDL and create a window and renderer

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window* main_window = SDL_CreateWindow(
        app_title.c_str(),
        SDL_WINDOWPOS_CENTERED, // Use Centered Top Coordinate
        SDL_WINDOWPOS_CENTERED, // Use Centered Left Coordinate
        1024, 768,              // Screen's Width & Height (Drawable Area)
        0                       // Flags (Window is implicitly shown if SDL_WINDOW_HIDDEN is not set)
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(main_window, -1, 0);

    // PRIMARY & EVENT LOOP:
    while (!should_quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                should_quit = 1;
                break;
            }
        }

        // Rendering Here...
    }

    // CLEAN-UP & SHUTDOWN:
    // -----------------------------------------------------------------------------------------------------------------
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

    return 0;
}