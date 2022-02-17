#include "clove.h"
#include "../includes/game_basics.h"

CLOVE_TEST(MovableFixPositionX)
{
    level_t level;
    int32_t map[] = {0, 0, 0, 0};
    level_init(&level, 4, 4, 8);
    level_create(&level, map);
    movable_t movable = {1, 0, 4, 4};
    move_on_level(&level, &movable, -10, 0);
    CLOVE_INT_EQ(movable.x, 0);
}

CLOVE_TEST(MovableFixPositionY)
{
    level_t level;
    int32_t map[] = {0, 0, 0, 0};
    level_init(&level, 4, 4, 8);
    level_create(&level, map);
    movable_t movable = {1, 0, 4, 4};
    move_on_level(&level, &movable, 0, -10);
    CLOVE_INT_EQ(movable.y, 0);
}

CLOVE_TEST(MovableFixPositionWidth)
{
    level_t level;
    int32_t map[] = {0, 0, 0, 0};
    level_init(&level, 4, 4, 8);
    level_create(&level, map);
    movable_t movable = {1, 0, 4, 4};
    move_on_level(&level, &movable, 50, 0);
    CLOVE_FLOAT_EQ(movable.x, 28);
}

CLOVE_TEST(MovableFixPositionHeight)
{
    level_t level;
    int32_t map[] = {0, 0, 0, 0};
    level_init(&level, 4, 4, 8);
    level_create(&level, map);
    movable_t movable = {1, 0, 4, 4};
    move_on_level(&level, &movable, 0, 100);
    CLOVE_FLOAT_EQ(movable.y, 28);
}

CLOVE_TEST(MovableWallHit)
{
    level_t level;
    int32_t map[] = {0, 0x101, 0, 0};
    level_init(&level, 2, 2, 64);
    level_create(&level, map);
    movable_t movable = {1, 1, 32, 32};
    move_on_level(&level, &movable, 80, 0);
    CLOVE_FLOAT_EQ(movable.x, 32);
}

CLOVE_TEST(MovableWallHitHead)
{
    level_t level;
    int32_t map[] = {0, 0x0101, 0, 0};
    level_init(&level, 2, 2, 64);
    level_create(&level, map);
    movable_t movable = { 0, 64, 32, 32};
    move_on_level(&level, &movable, 70, 0);
    CLOVE_FLOAT_EQ(movable.x, 32);
}

CLOVE_TEST(MovableWallHitFeet)
{
    level_t level;
    int32_t map[] = {0x0101, 0, 0, 0};
    level_init(&level, 4, 4, 64);
    level_create(&level, map);
    movable_t movable = { 0, 64, 32, 32};
    move_on_level(&level, &movable, 0, -10);
    CLOVE_FLOAT_EQ(movable.y, 64);
}

CLOVE_SUITE(MovableSuite)
{
    CLOVE_SUITE_TESTS(
        MovableFixPositionX,
        MovableFixPositionY,
        MovableFixPositionWidth,
        MovableFixPositionHeight,
        MovableWallHit,
        MovableWallHitHead,
        MovableWallHitFeet
    );
}