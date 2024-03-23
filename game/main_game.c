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

Overworld *generate_overworld() {
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
                for (int l = 0; l < SCREEN_COLS; l++) {
                    overworld->screens[i][j].tiles[k][l] = map[rand() % MAP_HEIGHT][rand() % MAP_WIDTH];
                }
            }
        }
    }
    return overworld;
}

void display_screen(Screen *screen) {
    for (int i = 0; i < SCREEN_ROWS; i++) {
        for (int j = 0; j < SCREEN_COLS; j++) {
            printf("%c", screen->tiles[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    srand(time(NULL));

    generate_map();

    Overworld *overworld = generate_overworld();

    int current_screen_x = overworld->pos_x;
    int current_screen_y = overworld->pos_y;

    display_screen(&overworld->screens[current_screen_y][current_screen_x]);

    char direction;
    while (1) {
        scanf("%c", &direction);
        if (direction == 'q') {
            break;
        } else if (direction == 'w' && current_screen_y > 0) {
            current_screen_y--;
        } else if (direction == 's' && current_screen_y < MAP_ROWS - 1) {
            current_screen_y++;
        } else if (direction == 'a' && current_screen_x > 0) {
            current_screen_x--;
        } else if (direction == 'd' && current_screen_x < MAP_COLS - 1) {
            current_screen_x++;
        }
        display_screen(&overworld->screens[current_screen_y][current_screen_x]);
    }

    return 0;
}
