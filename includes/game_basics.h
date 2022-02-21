#pragma once
#include <stdint.h>

#define BLOCK_MASK_UNWALKABLE 0x0100

#define BLOCK_GROUND 0
#define BLOCK_LIMIT 1
#define BLOCK_WALL 2
#define BLOCK_PLAYER1WALL 4
#define BLOCK_PLAYER2WALL 8

typedef struct level{

    uint32_t cols; // Number of colums in the grid
    uint32_t rows; // Number of rows in the grid 
    int32_t cell_size; // Size in pixels of a single cell
    int32_t *cells; // Content of the level
    int32_t free_walls; // number of unconquered walls

} level_t;

typedef struct movable{

    float x;
    float y;
    float speed;
    uint32_t width;
    uint32_t height;

} movable_t;

typedef struct player{
    
    uint32_t id;
    uint32_t Wall_color;
    movable_t movable;
    uint8_t color_r;
    uint8_t color_g;
    uint8_t color_b;

} player_t;

// Initialize a new level
int level_init(level_t *level, const uint32_t cols, const uint32_t rows, const uint32_t cell_size);

// Get cell content at specified coordinates
int32_t level_cell(level_t *level, const uint32_t col, const uint32_t row);

// Procedural creation of level mapping
int32_t level_create(level_t *level, int32_t level_cells[]);

// Move an object on the level
int32_t move_on_level(level_t *level, movable_t *movable, const float delta_x, const float delta_y);

// Detect if (and which) a wall was collided
void detect_collision(level_t *level, player_t *player, const float delta_x, const float delta_y);

// Get player ID
int movable_get_id(player_t *player);

// Get current player X
float movable_get_x(player_t *player);

// Get current player Y
float movable_get_y(player_t *player);
