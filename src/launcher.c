#include "../includes/launcher.h"
#include "../includes/game_tuning.h"

int check_click_in_rect(int x, int y, SDL_Rect *rect)
{
    /* Check X coordinate is within rectangle range */
    if (x >= rect->x && x < (rect->x + rect->w))
    {
        /* Check Y coordinate is within rectangle range */
        if (y >= rect->y && y < (rect->y + rect->h))
        {
            /* X and Y is inside the rectangle */
            return 1;
        }
    }

    /* X or Y is outside the rectangle */
    return 0;
}

int start_launcher()
{
    // Create a window and a renderer for game launcher
    SDL_Window *launcher = SDL_CreateWindow("WallPainter! - launcher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(!launcher)
    {
        SDL_Log("Unable to open window: %s", SDL_GetError());
        return -1;
        SDL_Quit();
    }

    SDL_Renderer *launcher_renderer = SDL_CreateRenderer(launcher, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!launcher_renderer)
    {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        return -1;
        SDL_Quit();
    }

    // Launcher loop
    SDL_Surface* image = SDL_LoadBMP("assets/map_dimension.bmp");	
    SDL_Texture * texture = SDL_CreateTextureFromSurface(launcher_renderer, image);
    SDL_Rect dstrect = { 5, 5, 300, 80 };


    SDL_Rect small_rect = {WINDOW_WIDTH / 8, (WINDOW_HEIGHT / 2) - 32, 64, 64};
    SDL_Rect medium_rect = {WINDOW_WIDTH / 4, (WINDOW_HEIGHT / 2) - 64, 128, 128};
    SDL_Rect large_rect = {WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) - 128, 256, 256};
    int launcher_run = 1;
    int dimension = 0;

    while(launcher_run)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {               
                launcher_run = 0;
                // Destroy after usage
                if(launcher_renderer)
                {
                    SDL_DestroyRenderer(launcher_renderer);
                }
                if(launcher)
                {
                    SDL_DestroyWindow(launcher);
                }
                if(texture)
                {
                    SDL_DestroyTexture(texture);
                }
                if(image)
                {
                    SDL_FreeSurface(image);
                }
                SDL_Quit();
            }

            SDL_SetRenderDrawColor(launcher_renderer, 0, 0, 0, 0);
            SDL_RenderClear(launcher_renderer);

            if(check_click_in_rect(event.motion.x, event.motion.y, &small_rect))
            {
                SDL_SetRenderDrawColor(launcher_renderer, 00, 115, 51, 255);
                SDL_RenderFillRect(launcher_renderer, &small_rect);
                if(event.type == SDL_MOUSEBUTTONDOWN)
                {
                    dimension = MAP_SMALL;
                    launcher_run = 0;
                }
            }
            else if(check_click_in_rect(event.motion.x, event.motion.y, &medium_rect))
            {
                SDL_SetRenderDrawColor(launcher_renderer, 184, 115, 00, 255);
                SDL_RenderFillRect(launcher_renderer, &medium_rect);
                if(event.type == SDL_MOUSEBUTTONDOWN)
                {
                    dimension = MAP_MEDIUM;
                    launcher_run = 0;
                }
            }
            else if(check_click_in_rect(event.motion.x, event.motion.y, &large_rect))
            {
                SDL_SetRenderDrawColor(launcher_renderer, 184, 00, 51, 255);
                SDL_RenderFillRect(launcher_renderer, &large_rect);
                if(event.type == SDL_MOUSEBUTTONDOWN)
                {
                    dimension = MAP_LARGE;
                    launcher_run = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(launcher_renderer, 184, 115, 51, 255);
        SDL_RenderDrawRect(launcher_renderer, &small_rect);
        SDL_RenderDrawRect(launcher_renderer, &medium_rect);
        SDL_RenderDrawRect(launcher_renderer, &large_rect);
        SDL_RenderCopy(launcher_renderer, texture, NULL, &dstrect);

        SDL_RenderPresent(launcher_renderer);
    }

    // Destroy after usage
    if(launcher_renderer)
    {
        SDL_DestroyRenderer(launcher_renderer);
    }
    if(launcher)
    {
        SDL_DestroyWindow(launcher);
    }
    if(texture)
    {
        SDL_DestroyTexture(texture);
    }
    if(image)
    {
        SDL_FreeSurface(image);
    }

    return dimension;  
}