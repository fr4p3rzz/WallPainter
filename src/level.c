#include "../includes/game_basics.h"
#include "../includes/game_tuning.h"
#include "../includes/mapping.h"
#include <stdlib.h>
#include <stdio.h>
extern int p1_score;
extern int p2_score;

#define P1_COORDINATES (((PLAYER_1_SPAWN_Y  + PLAYER_HEIGHT - 1) / level->cell_size) * level->cols + PLAYER_1_SPAWN_X / level->cell_size)
#define P2_COORDINATES (((PLAYER_2_SPAWN_Y  + PLAYER_HEIGHT - 1) / level->cell_size) * level->cols + PLAYER_2_SPAWN_X / level->cell_size)
#define P1_POSITION_CURRENT (P1_COORDINATES == i)
#define P2_POSITION_CURRENT (P2_COORDINATES == i)



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
    int r = 0;
    uint32_t p2_cell_x = PLAYER_2_SPAWN_X / level->cell_size; // cell on which the movable will end
    uint32_t p2_cell_y = (PLAYER_2_SPAWN_Y  + PLAYER_HEIGHT - 1) / level->cell_size; // test for feet
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
                r = rand() % 100;
                if(P1_POSITION_CURRENT || P2_POSITION_CURRENT)
                {
                    level_cells[i] = 0; // Create a floor cell
                }
                else if((r < WALLS_MIN_SPAWN_SEED || r > WALLS_MAX_SPAWN_SEED) && r % 2 == 0 && level_cells[i-1] != (D))
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

void level_scorecount(level_t *level, int32_t cell, player_t *player)
{
    if(cell && player)
    {
        if(cell & BLOCK_WALL)
        {
            level->free_walls -=1;
            if(player->id == PLAYER_1_ID)
            {
                p1_score += 1;
            } 
            else
            {
                p2_score += 1;
            }
        }
        else if( cell & BLOCK_PLAYER1WALL) 
        {
            if(player->id != PLAYER_1_ID)
            {
                p1_score -= 1;
                p2_score += 1;
            }
        }
        else if(cell & BLOCK_PLAYER2WALL)
        {
            if(player->id != PLAYER_2_ID)
            {
                p2_score -= 1;
                p1_score += 1;
            }
        }
    }
}