#include <SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // https://github.com/nothings/stb/blob/master/stb_image.h

int main(int argc, char* argv[])
{
    std::string app_title = "SDL_Guide 03: Image Loading & Rendering";
    int display_width = 1024;
    int display_height = 768;
    std::stringstream error;
    SDL_Window* main_window = NULL;
    SDL_Renderer* renderer = NULL;
    std::string image_path = "../assets/green-chameleon-01.png";
    SDL_Texture* image_texture = NULL;
    SDL_Rect image_rect = { 0, 0 };

    try {
        // CONFIGURE LOGGING:
        // -----------------------------------------------------------------------------------------------------------------

        // Set this so that all logged messages show in the console
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

        // INITIALIZATION:
        // -----------------------------------------------------------------------------------------------------------------
        // Initialize SDL and create a window and renderer

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initializing SDL...");
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
            error << "Failed to initialize SDL: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Creating main window...");
        if ((main_window = SDL_CreateWindow(
            app_title.c_str(),
            SDL_WINDOWPOS_CENTERED,         // Use Centered Top Coordinate
            SDL_WINDOWPOS_CENTERED,         // Use Centered Left Coordinate
            display_width, display_height,  // Width & Height
            0                               // Flags (Window is implicitly shown if SDL_WINDOW_HIDDEN is not set)
        )) == 0) {
            error << "Failed to create a window: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Creating renderer...");
        if ((renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED)) == 0) {
            error << "Failed to create the renderer: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }

        // LOAD ASSETS & CREATE TEXTURES:
        // -----------------------------------------------------------------------------------------------------------------
        // I'm using stb_image to load various image formats, which is available here:
        // https://github.com/nothings/stb/blob/master/stb_image.h

        int orig_format;                        // What format was the file in? stbi_load requires this, but we do not need this information
        int depth = 32;                         // Each pixel has 4 channels and each channel is a byte (so 8 bits X 4 channels)
        int channels = STBI_rgb_alpha;          // 4 channels
        //int pitch = channels * image_rect.w;    // Pitch is the width of an entire row in bytes, so multiply image width by number of channels

        SDL_Log("Loading image from '%s'", image_path.c_str());
        unsigned char* data = stbi_load(image_path.c_str(), &image_rect.w, &image_rect.h, &orig_format, channels);
        if (data == NULL) {
            error << "Loading image failed: " << stbi_failure_reason();
            throw(std::exception(error.str().c_str()));
        }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Uint32 rmask = 0xff000000, gmask = 0x00ff0000, bmask = 0x0000ff00, amask = 0x000000ff;
#else // little endian, like x86, and typical desktop processors
        Uint32 rmask = 0x000000ff, gmask = 0x0000ff00, bmask = 0x00ff0000, amask = 0xff000000;
#endif
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Creating surface for image...");
        SDL_Surface* image_surface = SDL_CreateRGBSurfaceFrom(
            data,                       // The pixel buffer as loaded from stbi_load
            image_rect.w, image_rect.h, // Width and height of the image
            depth,                      // Depth is the number of bits per pixel
            channels * image_rect.w,    // Pitch is the number of bytes per image row
            rmask, gmask, bmask, amask  // So SDL knows which order the channels are in per 32-bits
        );
        if (image_surface == NULL) {
            stbi_image_free(data);      // Free the pixel buffer so that it doesn't leak after failing to create the surface
            error << "Creating image surface failed: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Creating texture for image and freeing surface...");
        image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
        SDL_FreeSurface(image_surface); // Free the surface because it's no longer needed after a texture is created
        stbi_image_free(data);          // Free the pixel buffer becausae SDL_FreeSurface doesn't do this for us
        if (image_texture == NULL) {
            error << "Creating image texture failed: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }

        // PRIMARY & EVENT LOOP:
        // -----------------------------------------------------------------------------------------------------------------
        // There are two loops (one nested into the other). If the event queue isn't emptied entirely for every frame
        // then events can get backed up in the queue and begin to lag, causing undesired effects

        bool should_quit = false;   // Used so the game-loop can end naturally
        SDL_Event event = { 0 };    // Create the event struct here so that it's not recreated repeatedly in the loop

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Game loop starting...");
        while (!should_quit) {
            // We'll discuss events later, but without the event loop, some platforms may not render or display  a window
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
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            // Render the loaded image:
            SDL_RenderCopy(renderer, image_texture,
                NULL,   // Use the entire image for the source rect
                NULL    // Stretch the image onto the entire display
            );

            // Show what was previously rendered to the back buffer:
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
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Freeing resources");
    if (image_texture) SDL_DestroyTexture(image_texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (main_window) SDL_DestroyWindow(main_window);
    SDL_Quit();
    SDL_Log("Shutdown");

    return 0;
}