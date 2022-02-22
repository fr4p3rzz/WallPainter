#include <SDL.h>
#include <time.h>
#include <stdio.h>
#include "./includes/launcher.h"
#include "./includes/game_basics.h"
#include "./includes/game_tuning.h"
#include "./includes/player_input.h"
#include "./includes/mapping.h"
#include "./includes/audio.h"

// Score for players
int p1_score = 0;
int p2_score = 0;

int main(int argc, char **argv)
{
    int game = 1;
    while(game)
    {
        // Setting the level
        level_t level;
        srand(time(NULL)); 

        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        {
            SDL_Log("Unable to initialize SDL, %s", SDL_GetError());
            return -1;
        }
        // Initialize audio
        initAudio();

        int dimension = start_launcher();
        switch (dimension)
        {
        case MAP_SMALL:
            int32_t level_cells_small[MAP_SM_GRID];
            level_init(&level, MAP_SMALL, MAP_SMALL, CELL_DIM);
            level_create(&level, level_cells_small);
            break;
        case MAP_MEDIUM:
            int32_t level_cells_medium[MAP_MD_GRID];
            level_init(&level, MAP_MEDIUM, MAP_MEDIUM, CELL_DIM);
            level_create(&level, level_cells_medium);
            break;
        case MAP_LARGE:
            int32_t level_cells_large[MAP_LG_GRID];
            level_init(&level, MAP_LARGE, MAP_LARGE, CELL_DIM);
            level_create(&level, level_cells_large);
            break;
        default:
            int32_t level_cells_default[MAP_SM_GRID];
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

        // Setting the player
        player_t player_1;
        player_1.id = PLAYER_1_ID;
        player_1.Wall_color = P1;
        player_1.movable.x = PLAYER_1_SPAWN_X;
        player_1.movable.y = PLAYER_1_SPAWN_Y;
        player_1.movable.width = PLAYER_WIDTH;
        player_1.movable.height = PLAYER_HEIGHT;
        player_1.movable.speed = PLAYER_1_SPEED;
        player_1.color_r = PLAYER_1_RCOLOR;
        player_1.color_g = PLAYER_1_GCOLOR;
        player_1.color_b = PLAYER_1_BCOLOR;

        player_t player_2;
        player_2.id = PLAYER_2_ID;
        player_2.Wall_color = P2;
        player_2.movable.x = PLAYER_2_SPAWN_X;
        player_2.movable.y = PLAYER_2_SPAWN_Y;
        player_2.movable.width = PLAYER_WIDTH;
        player_2.movable.height = PLAYER_HEIGHT;
        player_2.movable.speed = PLAYER_2_SPEED;
        player_2.color_r = PLAYER_2_RCOLOR;
        player_2.color_g = PLAYER_2_GCOLOR;
        player_2.color_b = PLAYER_2_BCOLOR;

        // Game loop validation
        int running = 1;
        int score_visual = 0;

        // Grid and player boxes
        SDL_Rect cell_rect = {0, 0, level.cell_size, level.cell_size};
        SDL_Rect player_1_rect = {0, 0, player_1.movable.width, player_1.movable.height};
        SDL_Rect player_2_rect = {0, 0, player_2.movable.width, player_2.movable.height};
        coordinates_t player_1_coordinates;
        coordinates_t player_2_coordinates;

        // Values for players movement
        float p1_delta_up = 0;
        float p1_delta_down = 0;
        float p1_delta_right = 0;
        float p1_delta_left = 0;

        float p2_delta_up = 0;
        float p2_delta_down = 0;
        float p2_delta_right = 0;
        float p2_delta_left = 0;

        int p1_collided = 0;
        int p2_collided = 0;

        int background = rand() % 256; // Random seed for ground color
        playMusic("assets/sounds/livinginsidethewalls.wav", SDL_MIX_MAXVOLUME / 5); // Start level music

        // Game loop
        while(running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
                player_1_coordinates = movement_input(event, player_1, &p1_delta_up, &p1_delta_down, &p1_delta_right, &p1_delta_left);
                player_2_coordinates = movement_input(event, player_2, &p2_delta_up, &p2_delta_down, &p2_delta_right, &p2_delta_left);
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);


            for(uint32_t row = 0; row < level.rows; row++) // For each row
            {
                for(uint32_t col = 0; col < level.cols; col++) // For each column
                {
                    int32_t cell = level_cell(&level, col, row);
                    int32_t cell_texture = cell & 0xff;
                    cell_rect.x = col * level.cell_size;
                    cell_rect.y = row * level.cell_size;


                    // Check the current status of each cell and color it correclty
                    if(cell_texture == BLOCK_GROUND)
                    {
                        SDL_SetRenderDrawColor(renderer, background, background * 0.647, background / 3, 255);
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
                    else if(cell_texture == BLOCK_PLAYER2WALL)
                    {
                        SDL_SetRenderDrawColor(renderer, player_2.color_r, player_2.color_g, player_2.color_b, 255);
                        SDL_RenderFillRect(renderer, &cell_rect);
                    }
                    else if(cell_texture == BLOCK_PLAYER1WALL)
                    {
                        SDL_SetRenderDrawColor(renderer, player_1.color_r, player_1.color_g, player_1.color_b, 255);
                        SDL_RenderFillRect(renderer, &cell_rect);
                    }
                }
            }

            move_on_level(&level, &player_1.movable, player_1_coordinates.x, player_1_coordinates.y);
            move_on_level(&level, &player_2.movable, player_2_coordinates.x, player_2_coordinates.y);
            int p1_collided = detect_collision(&level, &player_1, player_1_coordinates.x, player_1_coordinates.y);
            int p2_collided = detect_collision(&level, &player_2, player_2_coordinates.x, player_2_coordinates.y);
            if(p1_collided || p2_collided)
            {
                playSound("assets/sounds/laser.wav", SDL_MIX_MAXVOLUME / 7); // Play the score sound effect if a wall is collided and conquered
            }



            player_1_rect.x = player_1.movable.x;
            player_1_rect.y = player_1.movable.y;
            SDL_SetRenderDrawColor(renderer, player_1.color_r, player_1.color_g, player_1.color_b, 255);
            SDL_RenderFillRect(renderer, &player_1_rect);

            player_2_rect.x = player_2.movable.x;
            player_2_rect.y = player_2.movable.y;
            SDL_SetRenderDrawColor(renderer, player_2.color_r, player_2.color_g, player_2.color_b, 255);
            SDL_RenderFillRect(renderer, &player_2_rect);

            if(level.free_walls == 0) // If there are no more free walls, the game ends
            {
                running = 0;
                score_visual = 1; // move to the score window
            }

            SDL_RenderPresent(renderer);
        }
        playMusic("assets/sounds/livinginsidethewalls.wav", 0); // stops level music
        
        SDL_Surface* victory_img = (p1_score > p2_score) ? SDL_LoadBMP("assets/blue_win.bmp") : SDL_LoadBMP("assets/red_win.bmp");	// Show the correct result based on who wins
        SDL_Texture * victory_texture = SDL_CreateTextureFromSurface(renderer, victory_img);
        if(!victory_texture)
        {
            SDL_Log("Unable to create texture: %s", SDL_GetError());
            goto quit;
        }
        SDL_Rect vct_rect = { 5, 5, 300, 200 };
        SDL_Surface* controls_img = SDL_LoadBMP("assets/endgame_controls.bmp");	
        SDL_Texture * controls_texture = SDL_CreateTextureFromSurface(renderer, controls_img);
        if(!controls_texture)
        {
            SDL_Log("Unable to create texture: %s", SDL_GetError());
            goto quit;
        }
        SDL_Rect ctrl_rect = { (level.cols * level.cell_size) - 450, (level.rows * level.cell_size) - 200, 450, 200 };
        
        if(score_visual)
        {
            playSound("assets/sounds/victoryfanfare.wav", SDL_MIX_MAXVOLUME / 3);
        }
        while(score_visual)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                {
                    score_visual = 0;
                    game = 0;
                }
                if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
                {
                    score_visual = 0;
                    running = 1;
                }
            }

            if(p1_score > p2_score)
            {
                SDL_SetRenderDrawColor(renderer, player_1.color_r, player_1.color_g, player_1.color_b, 0);
                SDL_RenderClear(renderer); 
            }
            else if(p1_score < p2_score)
            {
                SDL_SetRenderDrawColor(renderer, player_2.color_r, player_2.color_g, player_2.color_b, 0);
                SDL_RenderClear(renderer); 
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
                SDL_RenderClear(renderer); 
            }

            SDL_RenderCopy(renderer, victory_texture, NULL, &vct_rect);
            SDL_RenderCopy(renderer, controls_texture, NULL, &ctrl_rect);
            SDL_RenderPresent(renderer);
        }

        quit:
        {
            if(victory_texture)
            {
                SDL_DestroyTexture(victory_texture);
            }
            if(controls_texture)
            {
                SDL_DestroyTexture(controls_texture);
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
    }

    endAudio();
    SDL_Quit();
    return 0;
}