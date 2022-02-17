#include "../includes/game_basics.h"
#include "../includes/level.h"
#include "../includes/game_tuning.h"
#include <stdlib.h>


int level_init(level_t *level, const uint32_t cols, const uint32_t rows, const uint32_t cell_size)
{
    if(!level || !cols || !rows || !cell_size)
    {
        return -1;
    }

    level->cols = cols;
    level->rows = rows;
    level->cell_size = cell_size;
    //level->cells = cells;

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
                level_cells[i] = W;
            }
            else
            {
                int r = rand() % 100;
                if(r < 15 || r > 90  && x > PLAYER_SPAWN_X - level->cell_size && x < PLAYER_SPAWN_X + level->cell_size && y > PLAYER_SPAWN_Y - level->cell_size && y < PLAYER_SPAWN_Y + level->cell_size)
                {
                    level_cells[i] = D;
                }
                else
                {
                    level_cells[i] = 0;
                }
            }
            i++;
        }
    }

    level->cells = level_cells;    

    return 0;
}
