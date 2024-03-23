#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "raylib.h"

#define MAP_WIDTH 256
#define MAP_HEIGHT 88
#define TILE_SIZE 8

typedef enum {
    GRASS,
    SAND,
    WATER,
    OBSTACLE_GREEN,
    OBSTACLE_BROWN,
} TerrainType;

int generate_map();

extern TerrainType map[MAP_HEIGHT][MAP_WIDTH];
