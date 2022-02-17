#include <SDL.h>
#include <time.h>
#include <stdio.h>
#include "./includes/launcher.h"
#include "./includes/game_basics.h"
#include "./includes/game_tuning.h"
#include "./includes/level.h"
#include "./includes/player_input.h"

int main(int argc, char **argv)
{

    // Setting the level
    level_t level;
    srand(time(NULL)); 


    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL, %s", SDL_GetError());
        return -1;
    }

    int dimension = start_launcher();
    switch (dimension)
    {
    case MAP_SMALL:
        int32_t level_cells_small[MAP_SMALL * MAP_SMALL];
        level_init(&level, MAP_SMALL, MAP_SMALL, CELL_DIM);
        level_create(&level, level_cells_small);
        break;
    case MAP_MEDIUM:
        int32_t level_cells_medium[MAP_MEDIUM * MAP_MEDIUM];
        level_init(&level, MAP_MEDIUM, MAP_MEDIUM, CELL_DIM);
        level_create(&level, level_cells_medium);
        break;
    case MAP_LARGE:
        int32_t level_cells_large[MAP_LARGE * MAP_LARGE];
        level_init(&level, MAP_LARGE, MAP_LARGE, CELL_DIM);
        level_create(&level, level_cells_large);
        break;
    default:
        int32_t level_cells_default[MAP_SMALL * MAP_SMALL];
        level_init(&level, MAP_SMALL, MAP_SMALL, CELL_DIM);
        level_create(&level, level_cells_default);
        break;
    }
    

    SDL_Window *window = SDL_CreateWindow("WallPainter!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, level.cols * level.cell_size, level.rows * level.cell_size, 0);
    if(!window)
    {
        SDL_Log("Unable to open window: %s", SDL_GetError());
        goto quit;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        goto quit;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, 128,  128);
    if(!texture)
    {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }

    // Setting the player
    player_t player;
    player.movable.x = PLAYER_SPAWN_X;
    player.movable.y = PLAYER_SPAWN_Y;
    player.movable.width = PLAYER_WIDTH;
    player.movable.height = PLAYER_HEIGHT;
    player.movable.speed = PLAYER_SPEED;
    player.color_r = 255;
    player.color_g = 255;
    player.color_b = 255;

    //Game loop validation
    int running = 1;

    // Grid and player boxes
    SDL_Rect cell_rect = {0, 0, level.cell_size, level.cell_size};
    SDL_Rect trail_rect = {0, 0, level.cell_size, level.cell_size};
    SDL_Rect player_rect = {0, 0, player.movable.width, player.movable.height};
    coordinates_t player_coordinates;

    // Values for player movement
    float delta_up = 0;
    float delta_down = 0;
    float delta_right = 0;
    float delta_left = 0;


    while(running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = 0;
            }
            player_coordinates = movement_input(event, player, &delta_up, &delta_down, &delta_right, &delta_left);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        for(uint32_t row = 0; row < level.rows; row++)
        {
            for(uint32_t col = 0; col < level.cols; col++)
            {
                int32_t cell = level_cell(&level, col, row);
                int32_t cell_texture = cell & 0xff;
                cell_rect.x = col * level.cell_size;
                cell_rect.y = row * level.cell_size;

                if(cell_texture == BLOCK_GROUND)
                {
                    SDL_SetRenderDrawColor(renderer, 184, 115, 51, 255);
                    SDL_RenderFillRect(renderer, &cell_rect);
                }
                else if(cell_texture == BLOCK_LIMIT)
                {
                    SDL_SetRenderDrawColor(renderer, 225, 193, 110, 255);
                    SDL_RenderFillRect(renderer, &cell_rect);
                }
                else if(cell_texture == BLOCK_WALL)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                    SDL_RenderFillRect(renderer, &cell_rect);
                }
            }
        }

        move_on_level(&level, &player.movable, player_coordinates.x, player_coordinates.y);

        player_rect.x = player.movable.x;
        player_rect.y = player.movable.y;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &player_rect);

        SDL_RenderPresent(renderer);
    }

    quit:
    {
        if(texture)
        {
            SDL_DestroyTexture(texture);
        }
        if(renderer)
        {
            SDL_DestroyRenderer(renderer);
        }
        if(window)
        {
            SDL_DestroyWindow(window);
        }  
    }
 

    SDL_Quit();
    return 0;
}