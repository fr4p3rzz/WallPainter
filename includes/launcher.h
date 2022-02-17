#pragma once
#include <SDL.h>

// Check if a given coordinate is inside a rect
int check_click_in_rect(int x, int y, struct SDL_Rect *rect);

// Start game launcher
int start_launcher();