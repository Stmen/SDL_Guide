#include <SDL.h>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[])
{
    std::string app_title = "SDL_Guide 02: Failure Checks & Logging";
    std::stringstream error;
    SDL_Window* main_window = NULL;
    SDL_Renderer* renderer = NULL;

    try {
        // CONFIGURE LOGGING:
        // -----------------------------------------------------------------------------------------------------------------

        // Set this so that all logged messages show in the console
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

        // INITIALIZATION:
        // -----------------------------------------------------------------------------------------------------------------
        // Initialize SDL and create a window and renderer

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
            error << "Failed to initialize SDL: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }
        SDL_Log("Initialized SDL");

        if ((main_window = SDL_CreateWindow(
            app_title.c_str(),
            SDL_WINDOWPOS_CENTERED,         // Use Centered Top Coordinate
            SDL_WINDOWPOS_CENTERED,         // Use Centered Left Coordinate
            1024, 768,                      // Width & Height
            0                               // Flags (Window is implicitly shown if SDL_WINDOW_HIDDEN is not set)
        )) == 0) {
            error << "Failed to create a window: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }
        SDL_Log("Created Main Window");

        if ((renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED)) == 0) {
            error << "Failed to create the renderer: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }
        SDL_Log("Created SDL Renderer");

        // PRIMARY & EVENT LOOP:
        // -----------------------------------------------------------------------------------------------------------------

        bool should_quit = false;
        SDL_Event event = { 0 };

        while (!should_quit) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                    should_quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        should_quit = 1;
                        break;
                    }
                    break;
                }
            }

            // RENDERING:
            // -----------------------------------------------------------------------------------------------------------------

            // Clear the background:
            SDL_SetRenderDrawColor(renderer, 0, 255, 15, 255);
            SDL_RenderClear(renderer);

            // Show what was rendered onto screen:
            SDL_RenderPresent(renderer);

            // Note: After a call to SDL_RenderPresent, if no textures had been rendered, with verbose logging turned on, it's 
            // normal for the following error to show in the console:
            // SDL failed to get a vertex buffer for this Direct3D 9 rendering batch!
            // This is only seen when batching is supported, which right now is most likely only with Windows + Direct3D 9
        }

    }
    catch (std::exception ex) {
        // This is a simple way to show a message box, if main_window failed to create this will still work
        // since main_window will be NULL (the message box will just not have a parent):
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, app_title.c_str(), ex.what(), main_window);

        // Output the error to the console, if you have one
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, ex.what());
    }

    // CLEAN-UP & SHUTDOWN:
    // -----------------------------------------------------------------------------------------------------------------
    if (renderer) SDL_DestroyRenderer(renderer);
    if (main_window) SDL_DestroyWindow(main_window);
    SDL_Quit();
    SDL_Log("Shutdown");

    return 0;
}