#include <SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "SDL_RectEx.h"

int main(int argc, char* argv[])
{
	std::string app_title = "SDL Box Movement Example";
	int screen_width = 1024, screen_height = 768;
	SDL_Window* main_window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event = { 0 };
	bool should_quit = false;
	SDL_RectF box_destination = { };
	std::stringstream error;

	try {
		// INITIALIZATION:
		// -----------------------------------------------------------------------------------------------------------------
		// Initialize SDL and create a window and renderer

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
			error << "Failed to initialize SDL: " << SDL_GetError();
			throw(error.str());
		}

		if ((main_window = SDL_CreateWindow(
			std::string(app_title + " - Use the arrow keys!").c_str(),
			SDL_WINDOWPOS_CENTERED,			// Use Centered Top Coordinate
			SDL_WINDOWPOS_CENTERED,			// Use Centered Left Coordinate
			screen_width, screen_height,	// Width & Height
			0								// Flags (Window is implicitly shown if SDL_WINDOW_HIDDEN is not set)
		)) == 0) {
			error << "Failed to create a window: " << SDL_GetError();
			throw(error.str());
		}

		if ((renderer = SDL_CreateRenderer(main_window, -1, 0)) == 0) {
			error << "Failed to create the renderer: " << SDL_GetError();
			throw(error.str());
		}

		// CONFIGURE THE BOX / PLAYER, IT'S INITIAL LOCATION AND SIZE:
		// -----------------------------------------------------------------------------------------------------------------
		// We will primarily keep up with the box coordinates with box_x and box_y as doubles because when the frame-rate is 
		// extremely high, we need the extra precision.
		box_destination.w = 100.0f;
		box_destination.h = 100.0f;
		box_destination.x = screen_width / 2.0f - box_destination.w / 2.0f;
		box_destination.y = screen_height / 2.0f - box_destination.h / 2.0f;

		// Setup the initial beforeTime before the game loop starts up...
		// SDL_GetPerformanceCounter is in a unit of measurement only meaningful to this computer
		// SDL_GetPerformanceFrequency is a value to divide by to convert the counter to seconds
		double beforeTime = (double)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
		float speed = 1.0f; /* pixel per millisecond of movement */

		// PRIMARY & EVENT LOOP:
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

			// CALCULATE DELTA TIME:
			// -----------------------------------------------------------------------------------------------------------------
			// Get the current time by querying the performance counter and using the performance frequency to give it meaning 
			// (convert it to miliseconds)
			double currentTime = (double)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency() /* currentTime is tick counter in Seconds */;
			float deltaTime = static_cast<float>(currentTime - beforeTime) * 1000.0f /* How long it's been since the last frame in Milliseconds */;
			beforeTime = currentTime; /* Prime beforeTime for the next frame */

			// CHECK THE KEYBOARD STATE FOR MOVEMENT KEYS & MOVE THE BOX:
			// -----------------------------------------------------------------------------------------------------------------
			// If you had used events for this instead (SDL_KEYDOWN/UP) you would have onlybeen able to act on one key press at 
			// a time. This also uses min() and max() to keep the box on the screen. The box should not go beyond the boundaries 
			// of the screen.
			const Uint8* state = SDL_GetKeyboardState(NULL);
			if (state[SDL_SCANCODE_LEFT]) {
				box_destination.x = std::max(box_destination.x - speed * deltaTime, 0.0f);
			}
			if (state[SDL_SCANCODE_RIGHT]) {
				box_destination.x = std::min(box_destination.x + speed * deltaTime, screen_width - box_destination.w);
			}
			if (state[SDL_SCANCODE_UP]) {
				box_destination.y = std::max(box_destination.y - speed * deltaTime, 0.0f);
			}
			if (state[SDL_SCANCODE_DOWN]) {
				box_destination.y = std::min(box_destination.y + speed * deltaTime, screen_height - box_destination.h);
			}

			// RENDERING:
			// -----------------------------------------------------------------------------------------------------------------

			// Clear the background:
			SDL_SetRenderDrawColor(renderer, 0, 255, 115, 255);
			SDL_RenderClear(renderer);

			// Render the box:
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &((SDL_Rect)box_destination));

			// Show on screen:
			SDL_RenderPresent(renderer);
		}

	}
	catch (std::string error_str) {
		// This is a simple way to show a message box, if main_window failed to create this will still work
		// since main_window will be NULL (the message box will just not have a parent):
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, app_title.c_str(), error_str.c_str(), main_window);

		// Output the error to the console, if you have one
		std::cout << error_str << std::endl;
	}

	// CLEAN-UP & SHUTDOWN:
	// -----------------------------------------------------------------------------------------------------------------
	if (renderer) SDL_DestroyRenderer(renderer);
	if (main_window) SDL_DestroyWindow(main_window);
	SDL_Quit();

	return 0;
}