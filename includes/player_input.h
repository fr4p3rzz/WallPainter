#pragma once
#include <SDL.h>
#include "game_basics.h"

// Position of the player
typedef struct coordinates{
    float x;
    float y;
} coordinates_t;

// Takes the player movement input
coordinates_t movement_input(SDL_Event event, player_t player, float* delta_up, float* delta_down, float* delta_right, float* delta_left);