#include "clove.h"
#include "../includes/game_basics.h"


CLOVE_TEST(MovableFixPositionY)
{
    player_t player;
    
    init_player(&player, 100, 0x104, 10, 10, 100, 32, 32, 255, 255, 255);
    CLOVE_INT_EQ(movable.y, 0);
}


CLOVE_SUITE(PlayerSuite)
{
    CLOVE_SUITE_TESTS(

        LevelInitializationNULL
        );
}