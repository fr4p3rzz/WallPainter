#include "clove.h"
#include "../includes/game_basics.h"

CLOVE_TEST(LevelInitializationNULL)
{
    CLOVE_INT_EQ(level_init(NULL, 0, 0, 0), -1);

}

CLOVE_TEST(LevelInitializationZeroCols)
{
    level_t level;
    CLOVE_INT_EQ(level_init(&level, 0, 10, 10), -1);

}

CLOVE_TEST(LevelInitializationZeroRows)
{
    level_t level;
    CLOVE_INT_EQ(level_init(&level, 10, 0, 10), -1);

}

CLOVE_TEST(LevelInitializationZeroCellSize)
{
    level_t level;
    CLOVE_INT_EQ(level_init(&level, 10, 10, 0), -1);

}

CLOVE_TEST(LevelCellsContent)
{
    level_t level;
    int32_t map[] = {1, 2, 3, 4};
    level_init(&level, 2, 2, 1);
    level_create(&level, map);
    CLOVE_INT_EQ(level_cell(&level, 0, 0), 1);
}

CLOVE_TEST(LevelCellsOutOfBoundsCol)
{
    level_t level;
    int32_t map[] = {1, 2, 3, 4};
    level_init(&level, 2, 2, 1);
    level_create(&level, map);
    CLOVE_INT_EQ(level_cell(&level, 3, 0), -1);
}

CLOVE_TEST(LevelCellsOutOfBoundsRow)
{
    level_t level;
    int32_t map[] = {1, 2, 3, 4};
    level_init(&level, 2, 2, 1);
    level_create(&level, map);
    CLOVE_INT_EQ(level_cell(&level, 0, 3), -1);
}

CLOVE_TEST(LevelCellsOutOfBoundsAll)
{
    level_t level;
    int32_t map[] = {1, 2, 3, 4};
    level_init(&level, 2, 2, 1);
    level_create(&level, map);
    CLOVE_INT_EQ(level_cell(&level, 100, 100), -1);
}

CLOVE_SUITE(LevelSuite)
{
    CLOVE_SUITE_TESTS(

        LevelInitializationNULL,
        LevelInitializationZeroCols,
        LevelInitializationZeroRows,
        LevelInitializationZeroCellSize,
        LevelCellsContent,
        LevelCellsOutOfBoundsCol,
        LevelCellsOutOfBoundsRow,
        LevelCellsOutOfBoundsAll
        );
}