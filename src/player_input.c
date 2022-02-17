#include "../includes/player_input.h"

coordinates_t movement_input(SDL_Event event, player_t player, float* delta_up, float* delta_down, float* delta_right, float* delta_left)
{
    coordinates_t coordinates;

    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_RIGHT)
        {
            *delta_right = player.movable.speed * (1.0 / 60);
        }
        if (event.key.keysym.sym == SDLK_LEFT)
        {
            *delta_left = -player.movable.speed * (1.0 / 60);
        }
        if (event.key.keysym.sym == SDLK_DOWN)
        {
            *delta_down = player.movable.speed * (1.0 / 60);
        }
        if (event.key.keysym.sym == SDLK_UP)
        {
            *delta_up = -player.movable.speed * (1.0 / 60);
        }
    }
    if (event.type == SDL_KEYUP)
    {
        if (event.key.keysym.sym == SDLK_RIGHT)
        {
            *delta_right = 0;
        }
        if (event.key.keysym.sym == SDLK_LEFT)
        {
            *delta_left = 0;
        }
        if (event.key.keysym.sym == SDLK_DOWN)
        {
            *delta_down = 0;
        }
        if (event.key.keysym.sym == SDLK_UP)
        {
            *delta_up = 0;
        }
    }

    coordinates.x = *delta_right + *delta_left;
    coordinates.y = *delta_up + *delta_down;

    return coordinates;
}