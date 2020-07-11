#pragma once
#include <SDL.h> // Unnecessary if the source including this header already includes SDL.h
#include <cmath> // For round()

struct SDL_RectF {
	float x, y, w, h;

	operator SDL_Rect()
	{
		return SDL_Rect{
			static_cast<int>(std::round(x)),
			static_cast<int>(std::round(y)),
			static_cast<int>(std::round(w)),
			static_cast<int>(std::round(h)),
		};
	}
};

struct SDL_RectD {
	double x, y, w, h;

	operator SDL_Rect()
	{
		return SDL_Rect{
			static_cast<int>(std::round(x)),
			static_cast<int>(std::round(y)),
			static_cast<int>(std::round(w)),
			static_cast<int>(std::round(h)),
		};
	}
};