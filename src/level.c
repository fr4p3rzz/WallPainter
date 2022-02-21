#include "../includes/game_basics.h"
#include "../includes/game_tuning.h"
#include "../includes/mapping.h"
#include <stdlib.h>
#include <stdio.h>

#define P1_POSITION x > PLAYER_1_SPAWN_X - level->cell_size && x < PLAYER_1_SPAWN_X + level->cell_size && y > PLAYER_1_SPAWN_Y - level->cell_size && y < PLAYER_1_SPAWN_Y + level->cell_size
#define P2_POSITION x > PLAYER_2_SPAWN_X - level->cell_size && x < PLAYER_2_SPAWN_X + level->cell_size && y > PLAYER_2_SPAWN_Y - level->cell_size && y < PLAYER_2_SPAWN_Y + level->cell_size


int level_init(level_t *level, const uint32_t cols, const uint32_t rows, const uint32_t cell_size)
{
    if(!level || !cols || !rows || !cell_size)
    {
        return -1;
    }

    level->cols = cols;
    level->rows = rows;
    level->cell_size = cell_size;
    level->free_walls = 0;

    return 0;
}

int32_t level_cell(level_t *level, const uint32_t col, const uint32_t row)
{
    if (!level->cells || col >= level->cols || row >= level->rows)
    {
        return -1;
    }

    return level->cells[row * level->cols + col];
}

int32_t level_create(level_t *level, int32_t *level_cells)
{
    if(!level)
    {
        return -1;
    }
    if(!level_cells)
    {
        return -1;
    }

    int i = 0;
    for(int x = 0; x < level->rows; x++)
    {
        for(int y = 0; y < level->cols; y++)
        {
            if(x == 0 || x == level->rows -1 || y == 0 || y == level->cols -1)
            {
                level_cells[i] = W; // Create a limit wall cell along the borders
            }
            else
            {
                int r = rand() % 100;
                if(r < 15 || r > 90  && P1_POSITION && P2_POSITION)
                {
                    level_cells[i] = D; // Create a conquerable wall
                    level->free_walls += 1; // Increase the counter of this level's conquerable walls
                }
                else
                {
                    level_cells[i] = 0; // Create a floor cell
                }
            }
            i++;
        }

    }
    level->cells = level_cells;    

    return 0;
}