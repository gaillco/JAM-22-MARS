#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "../include/generator.h"
#include "../include/game.h"

#define MAP_ROWS 8
#define MAP_COLS 10
#define SCREEN_ROWS 11
#define SCREEN_COLS 32

typedef struct {
    TerrainType **tiles;
} Screen;

typedef struct {
    int pos_x;
    int pos_y;
    Screen **screens;
} Overworld;

void fill_overworld(Overworld *overworld) {
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            for (int k = 0; k < SCREEN_ROWS; k++) {
                for (int l = 0; l < SCREEN_COLS; l++) {
                    overworld->screens[i][j].tiles[k][l] = map[i][j];
                }
            }
        }
    }
}

Overworld *malloc_overworld() {
    Overworld *overworld = malloc(sizeof(Overworld));
    overworld->pos_x = rand() % MAP_COLS;
    overworld->pos_y = rand() % MAP_ROWS;
    overworld->screens = malloc(MAP_ROWS * sizeof(Screen *));
    for (int i = 0; i < MAP_ROWS; i++) {
        overworld->screens[i] = malloc(MAP_COLS * sizeof(Screen));
        for (int j = 0; j < MAP_COLS; j++) {
            overworld->screens[i][j].tiles = malloc(SCREEN_ROWS * sizeof(TerrainType *));
            for (int k = 0; k < SCREEN_ROWS; k++) {
                overworld->screens[i][j].tiles[k] = malloc(SCREEN_COLS * sizeof(TerrainType));
            }
        }
    }
    return overworld;
}

int main()
{
    srand(time(NULL));

    generate_map();

    Overworld *overworld = malloc_overworld();
    fill_overworld(overworld);

    return 0;
}
