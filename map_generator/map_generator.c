#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "raylib.h"
#include "../include/generator.h"

#define OBSTACLE_PROB 15
#define PERLIN_OFFSET_X 1500
#define PERLIN_OFFSET_Y 1500

void generate_random_walk(int startX, int startY, int steps) {
    int x = startX;
    int y = startY;

    for (int i = 0; i < steps; i++) {
        map[y][x] = GRASS;

        int direction = rand() % 4;
        switch (direction) {
            case 0:
                if (y > 0) y--;
                break;
            case 1:
                if (y < MAP_HEIGHT - 1) y++;
                break;
            case 2:
                if (x > 0) x--;
                break;
            case 3:
                if (x < MAP_WIDTH - 1) x++;
                break;
        }
    }
}

void assign_terrain_type(float **heatmap) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map[i][j] == GRASS) {
                if (rand() % 100 < OBSTACLE_PROB)
                    map[i][j] = OBSTACLE_GREEN;
                continue;
            }
            
            if (heatmap[i][j] < 0.3)
                map[i][j] = WATER;
            else if (heatmap[i][j] < 0.5) {
                map[i][j] = SAND;
                if (rand() % 100 < OBSTACLE_PROB)
                    map[i][j] = OBSTACLE_BROWN;
            }
            else {
                map[i][j] = GRASS;
                if (rand() % 100 < OBSTACLE_PROB)
                    map[i][j] = OBSTACLE_GREEN;
            }
        }
    }
}

float **get_image_data(Image perlinImage) {
    float **perlinNoise = malloc(MAP_HEIGHT * sizeof(float *));

    for (int i = 0; i < MAP_HEIGHT; i++) {
        perlinNoise[i] = malloc(MAP_WIDTH * sizeof(float));
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            Color color = GetImageColor(perlinImage, j, i);
            perlinNoise[i][j] = color.r / 255.0f;
        }
    }

    return perlinNoise;
}

int generate_map()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    long millis = (time.tv_sec * 1000) + (time.tv_usec / 1000);
    srand(millis);

    int offsetX = rand() % PERLIN_OFFSET_X;
    int offsetY = rand() % PERLIN_OFFSET_Y;

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map[i][j] = WATER;
        }
    }

    int steps = 7500 + rand() % (20000 - 7500 + 1);
    generate_random_walk(MAP_WIDTH + rand() % 100, MAP_HEIGHT + rand() % 100, steps);

    Image perlinImage = GenImagePerlinNoise(MAP_WIDTH + offsetX, MAP_HEIGHT + offsetY, 16, 16, 1.0f);
    float **perlinNoise = get_image_data(perlinImage);

    assign_terrain_type(perlinNoise);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        free(perlinNoise[i]);
    }
    free(perlinNoise);

    return 0;
}
