#include "game_basics.h"
/** VALUES FOR LAUNCHER */
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/** VALUES FOR GAME */
#define MAP_SMALL 16 // Dimension of the square matrix for small map
#define MAP_MEDIUM 24 // Dimension of the square matrix for medium map
#define MAP_LARGE 32 // Dimension of the square matrix for large map
#define MAP_SM_GRID MAP_SMALL * MAP_SMALL // This value is passed to the level builder; mixing values will change the layout
#define MAP_MD_GRID MAP_MEDIUM * MAP_MEDIUM // This value is passed to the level builder; mixing values will change the layout
#define MAP_LG_GRID MAP_LARGE * MAP_LARGE // This value is passed to the level builder; mixing values will change the layout
#define CELL_DIM 38 // Dimension of the single cell of the grid: change this value will affect the visual scale of the game
#define WALLS_MIN_SPAWN_SEED 30 // Increasing this will increase the spawn rate of paintable walls (formula: x < MIN OR x > MAX)
#define WALLS_MAX_SPAWN_SEED 85 // Decreasing this will increase the spawn rate of paintable walls (formula: x < MIN OR x > MAX)
#define PLAYER_HEIGHT 32 // Height of the player (changing this without changing the grid scale may cause problems, player should be smaller than grid cells)
#define PLAYER_WIDTH 32 // Width of the player (changing this without changing the grid scale may cause problems, player should be smaller than grid cells)
#define PLAYER_1_SPEED 256 // Movement speed for Player1
#define PLAYER_2_SPEED 256 // Movement speed for Player2
#define PLAYER_1_SPAWN_X 114 // X-coordinate for spawning player1 (should be multiple of CELL_DIM)
#define PLAYER_1_SPAWN_Y 152 // Y-coordinate for spawning player1 (should be multiple of CELL_DIM)
#define PLAYER_2_SPAWN_X 114 // X-coordinate for spawning player2 (should be multiple of CELL_DIM)
#define PLAYER_2_SPAWN_Y 190 // Y-coordinate for spawning player2 (should be multiple of CELL_DIM)
#define PLAYER_1_RCOLOR 000 // Amount of red for P1 color (0-255)
#define PLAYER_1_GCOLOR 000 // Amount of green for P1 color (0-255)
#define PLAYER_1_BCOLOR 255 // Amount of Blue for P1 color (0-255)
#define PLAYER_2_RCOLOR 255 // Amount of red for P2 color (0-255)
#define PLAYER_2_GCOLOR 000 // Amount of green for P2 color (0-255)
#define PLAYER_2_BCOLOR 000 // Amount of Blue for P2 color (0-255)
#define PLAYER_1_ID 0x001 // ID for P1 (internal use only, change this should not affect anything)
#define PLAYER_2_ID 0x002 // ID for P2 (internal use only, change this should not affect anything)


