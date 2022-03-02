#include "../includes/game_basics.h"
#include "../includes/game_tuning.h"
#include "../includes/mapping.h"

#define PLAYER_1_AUTOHIT (player->id == PLAYER_1_ID && (cell & BLOCK_PLAYER1WALL))
#define PLAYER_2_AUTOHIT (player->id == PLAYER_2_ID && (cell & BLOCK_PLAYER2WALL))

int32_t move_on_level(level_t *level, movable_t *movable, const float delta_x, const float delta_y)
{
    float new_x = movable->x + delta_x;
    float new_y = movable->y + delta_y;

    if (new_x < 0)
        new_x = 0;
    else if (new_x + movable->width >= level->cols * level->cell_size)
        new_x = (level->cols * level->cell_size) - movable->width;

    if (new_y < 0)
        new_y = 0;
    else if (new_y + movable->height >= level->rows * level->cell_size)
        new_y = (level->rows * level->cell_size) - movable->height;

    int32_t cell = -1;

    // moving right ?
    if (new_x > movable->x)
    {
        uint32_t cell_x = (new_x + movable->width - 1) / level->cell_size; // cell on which the movable will end
        uint32_t cell_y = (movable->y + movable->height - 1) / level->cell_size; // test for feet
        cell = level_cell(level, cell_x, cell_y);
        if (cell & BLOCK_MASK_UNWALKABLE) // collision!
        {
            movable->x = cell_x * level->cell_size - movable->width; // bring back
        }
        else
        {
            cell_y = movable->y / level->cell_size; // test for neck
            cell = level_cell(level, cell_x, cell_y);
            if (cell & BLOCK_MASK_UNWALKABLE) // collision!
            {
                movable->x = cell_x * level->cell_size - movable->width; // bring back
            }
            else
            {
                movable->x = new_x;
            }
        }
    }
    
    // moving left ?
    if (new_x < movable->x)
    {
        uint32_t cell_x = new_x / level->cell_size; // cell on which the movable will end
        uint32_t cell_y = (movable->y + movable->height - 1) / level->cell_size; // test for feet
        cell = level_cell(level, cell_x, cell_y);
        if (cell & BLOCK_MASK_UNWALKABLE) // collision!
        {
            movable->x = (cell_x * level->cell_size) + level->cell_size;
        }
        else
        {
            cell_y = movable->y / level->cell_size; // test for neck
            cell = level_cell(level, cell_x, cell_y);
            if (cell & BLOCK_MASK_UNWALKABLE) // collision!
            {
                movable->x = (cell_x * level->cell_size) + level->cell_size;
            }
            else
            {
                movable->x = new_x;
            }
        }
    }

    // moving down ?
    if(new_y > movable->y)
    {
        uint32_t cell_x = movable->x / level->cell_size; // test the left side
        uint32_t cell_y = (new_y + movable->height - 1) / level->cell_size;
        cell = level_cell(level, cell_x, cell_y);
        
        if (cell & BLOCK_MASK_UNWALKABLE)
        {
            movable->y = (cell_y * level->cell_size) - movable->height;
        }
        else
        {
            cell_x = (movable->x + movable->width -1) / level->cell_size; // test the right side
            cell = level_cell(level, cell_x, cell_y);
            if (cell & BLOCK_MASK_UNWALKABLE) // collision!
            {
                movable->y = (cell_y * level->cell_size) - movable->height;
            }
            else
            {
                movable->y = new_y;
            }
        }
    }

    // moving up ?
    if(new_y < movable->y)
    {
        uint32_t cell_x = movable->x / level->cell_size; // test the left side
        uint32_t cell_y = new_y / level->cell_size;
        cell = level_cell(level, cell_x, cell_y);

        if (cell & BLOCK_MASK_UNWALKABLE)
        {
            movable->y = (cell_y * level->cell_size) + level->cell_size;
        }
        else
        {
            cell_x = (movable->x + movable->width -1) / level->cell_size; // test the right side
            cell = level_cell(level, cell_x, cell_y);
            if (cell & BLOCK_MASK_UNWALKABLE) // collision!
            {
                movable->y = (cell_y * level->cell_size) + level->cell_size;
            }
            else
            {
                movable->y = new_y;
            }
        }
    }


    return cell;
}

int movable_get_id(player_t *player)
{
    return player->id;
}

float movable_get_x(player_t *player)
{
    return player->movable.x;
}

float movable_get_y(player_t *player)
{
    return player->movable.y;
}

int detect_collision(level_t *level, player_t *player, const float delta_x, const float delta_y)
{
    float new_x = player->movable.x + delta_x;
    float new_y = player->movable.y + delta_y;
    int32_t cell = -1;
    int collided = 0;
    
    // moving right ?
    if (new_x > player->movable.x)
    {
        uint32_t cell_x = (new_x + player->movable.width - 1) / level->cell_size; // cell on which the movable will end
        uint32_t cell_y = (player->movable.y + player->movable.height - 1) / level->cell_size; // test for feet
        cell = level_cell(level, cell_x, cell_y);
        if(!PLAYER_1_AUTOHIT && !PLAYER_2_AUTOHIT)
        {
            if (cell & BLOCK_MASK_UNWALKABLE && !(cell & BLOCK_LIMIT)) // collision with paintable wall!
            {
                level->cells[cell_y * level->cols + cell_x] = player->wall_color;
                level_scorecount(level, cell, player);    
                collided = 1; 
            }
            else
            {
                cell_y = player->movable.y / level->cell_size; // test for neck
                cell = level_cell(level, cell_x, cell_y);
                if (cell & BLOCK_MASK_UNWALKABLE && !(cell & BLOCK_LIMIT)) // collision with paintable wall!
                {
                    level->cells[cell_y * level->cols + cell_x] = player->wall_color;
                    level_scorecount(level, cell, player);
                    collided = 1; 
                }
            }
        }
    }
    
    // moving left ?
    if (new_x < player->movable.x)
    {
        uint32_t cell_x = new_x / level->cell_size; // cell on which the movable will end
        uint32_t cell_y = (player->movable.y + player->movable.height - 1) / level->cell_size; // test for feet
        cell = level_cell(level, cell_x, cell_y);
        if(!PLAYER_1_AUTOHIT && !PLAYER_2_AUTOHIT)
        {
            if (cell & BLOCK_MASK_UNWALKABLE && !(cell & BLOCK_LIMIT)) // collision with paintable wall!
            {
                level->cells[cell_y * level->cols + cell_x] = player->wall_color;
                level_scorecount(level, cell, player);
                collided = 1; 
            }
            else
            {
                cell_y = player->movable.y / level->cell_size; // test for neck
                cell = level_cell(level, cell_x, cell_y);
                if (cell & BLOCK_MASK_UNWALKABLE && !(cell & BLOCK_LIMIT)) // collision with paintable wall!
                {
                    level->cells[cell_y * level->cols + cell_x] = player->wall_color;
                    level_scorecount(level, cell, player);
                    collided = 1; 
                }
            }
        }
    }

    // moving down ?
    if(new_y > player->movable.y)
    {
        uint32_t cell_x = player->movable.x / level->cell_size; // test the left side
        uint32_t cell_y = (new_y + player->movable.height - 1) / level->cell_size;
        cell = level_cell(level, cell_x, cell_y);
        
        if(!PLAYER_1_AUTOHIT && !PLAYER_2_AUTOHIT)
        {
            if (cell & BLOCK_MASK_UNWALKABLE && !(cell & BLOCK_LIMIT))
            {
                level->cells[cell_y * level->cols + cell_x] = player->wall_color;
                level_scorecount(level, cell, player);
                collided = 1; 
            }
            else
            {
                cell_x = (player->movable.x + player->movable.width -1) / level->cell_size; // test the right side
                cell = level_cell(level, cell_x, cell_y);
                if (cell & BLOCK_MASK_UNWALKABLE && !(cell & BLOCK_LIMIT)) // collision with paintable wall!
                {
                    level->cells[cell_y * level->cols + cell_x] = player->wall_color;
                    level_scorecount(level, cell, player);
                    collided = 1; 
                }
            }
        }
    }

    // moving up ?
    if(new_y < player->movable.y)
    {
        uint32_t cell_x = player->movable.x / level->cell_size; // test the left side
        uint32_t cell_y = new_y / level->cell_size;
        cell = level_cell(level, cell_x, cell_y);

        if(!PLAYER_1_AUTOHIT && !PLAYER_2_AUTOHIT)
        {
            if (cell & BLOCK_MASK_UNWALKABLE && !(cell & BLOCK_LIMIT))
            {
                level->cells[cell_y * level->cols + cell_x] = player->wall_color;
                level_scorecount(level, cell, player);
                collided = 1; 
            }
            else
            {
                cell_x = (player->movable.x + player->movable.width -1) / level->cell_size; // test the right side
                cell = level_cell(level, cell_x, cell_y);
                if (cell & BLOCK_MASK_UNWALKABLE && !(cell & BLOCK_LIMIT)) // collision with paintable wall!
                {
                    level->cells[cell_y * level->cols + cell_x] = player->wall_color;
                    level_scorecount(level, cell, player);
                    collided = 1; 
                }
            }
        }
    }

    return collided;
}