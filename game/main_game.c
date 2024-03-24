#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "raylib.h"
#include "../include/generator.h"
#include "../include/game.h"

#define MAP_ROWS 8
#define MAP_COLS 16
#define SCREEN_ROWS 11
#define SCREEN_COLS 16
#define TEXTURE_SIZE 32

#define PLAYER_TEXTURE_PATH "game/assets/link.png"
#define GRASS_TEXTURE_PATH "game/assets/grass.png"
#define SAND_TEXTURE_PATH "game/assets/sand.png"
#define WATER_TEXTURE_PATH "game/assets/water.png"
#define OBSTACLE_GREEN_TEXTURE_PATH "game/assets/green_obstacle.png"
#define OBSTACLE_BROWN_TEXTURE_PATH "game/assets/brown_obstacle.png"

Texture2D textures[5];

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT
} directions;

typedef struct {
    Texture2D texture;
    Vector2 position;
    Vector2 size;
    int frame;
    int frame_counter;
    directions direction;
} Player;

Texture2D texture_goals;

typedef struct Goals_s {
    int pos_x;
    int pos_y;
    Vector2 position;
    bool acquired;
    struct Goals_s *next;
} Goals_t;

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
                    overworld->screens[i][j].tiles[k][l] = map[i * SCREEN_ROWS + k][j * SCREEN_COLS + l];
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

Player *init_player() {
    Player *player = malloc(sizeof(Player));
    player->texture = LoadTexture(PLAYER_TEXTURE_PATH);
    player->position = (Vector2) {0, 0};
    player->size = (Vector2) {TEXTURE_SIZE, TEXTURE_SIZE};
    player->frame = 0;
    player->frame_counter = 0;
    player->direction = DOWN;
    return player;
}

void load_textures()
{
    textures[GRASS] = LoadTexture(GRASS_TEXTURE_PATH);
    textures[SAND] = LoadTexture(SAND_TEXTURE_PATH);
    textures[WATER] = LoadTexture(WATER_TEXTURE_PATH);
    textures[OBSTACLE_GREEN] = LoadTexture(OBSTACLE_GREEN_TEXTURE_PATH);
    textures[OBSTACLE_BROWN] = LoadTexture(OBSTACLE_BROWN_TEXTURE_PATH);
}

void render_screen(Overworld *overworld, int pos_x, int pos_y)
{
    for (int i = 0; i < SCREEN_ROWS; i++) {
        for (int j = 0; j < SCREEN_COLS; j++) {
            switch (overworld->screens[pos_y][pos_x].tiles[i][j]) {
                case GRASS:
                    DrawTexture(textures[GRASS], j * TEXTURE_SIZE, i * TEXTURE_SIZE, WHITE);
                    break;
                case SAND:
                    DrawTexture(textures[SAND], j * TEXTURE_SIZE, i * TEXTURE_SIZE, WHITE);
                    break;
                case WATER:
                    DrawTexture(textures[WATER], j * TEXTURE_SIZE, i * TEXTURE_SIZE, WHITE);
                    break;
                case OBSTACLE_GREEN:
                    DrawTexture(textures[OBSTACLE_GREEN], j * TEXTURE_SIZE, i * TEXTURE_SIZE, WHITE);
                    break;
                case OBSTACLE_BROWN:
                    DrawTexture(textures[OBSTACLE_BROWN], j * TEXTURE_SIZE, i * TEXTURE_SIZE, WHITE);
                    break;
            }
        }
    }
}

void find_nearest_walkable_tile(Player *player, Overworld *overworld, int *pos_x, int *pos_y) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newX = player->position.x / TEXTURE_SIZE + j;
            int newY = player->position.y / TEXTURE_SIZE + i;
            if (newX >= 0 && newX < SCREEN_COLS && newY >= 0 && newY < SCREEN_ROWS) {
                TerrainType tileType = overworld->screens[*pos_y][*pos_x].tiles[newY][newX];
                if (tileType != OBSTACLE_GREEN && tileType != OBSTACLE_BROWN && tileType != WATER) {
                    player->position.x = newX * TEXTURE_SIZE;
                    player->position.y = newY * TEXTURE_SIZE;
                    return;
                }
            }
        }
    }
}

void move_player(Player *player, Overworld *overworld, int *pos_x, int *pos_y)
{
    Rectangle playerRec = { player->position.x, player->position.y, TEXTURE_SIZE, TEXTURE_SIZE };
    Rectangle tileRec;
    int moveX = 0, moveY = 0;

    if (IsKeyDown(KEY_UP)) {
        moveY = -2;
    }
    if (IsKeyDown(KEY_DOWN)) {
        moveY = 2;
    }
    if (IsKeyDown(KEY_LEFT)) {
        moveX = -2;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        moveX = 2;
    }

    playerRec.x += moveX;
    playerRec.y += moveY;

    for (int i = 0; i < SCREEN_ROWS; i++) {
        for (int j = 0; j < SCREEN_COLS; j++) {
            TerrainType tileType = overworld->screens[*pos_y][*pos_x].tiles[i][j];
            if (tileType == OBSTACLE_GREEN || tileType == OBSTACLE_BROWN || tileType == WATER) {
                tileRec = (Rectangle) { j * TEXTURE_SIZE, i * TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_SIZE };
                if (CheckCollisionRecs(playerRec, tileRec)) {
                    moveX = 0;
                    moveY = 0;
                    break;
                }
            }
        }
    }

    player->position.x += moveX;
    player->position.y += moveY;

    if (*pos_x == 0 && player->position.x < 0) {
        player->position.x = 0;
    } else if (*pos_x == MAP_COLS - 1 && player->position.x > (SCREEN_COLS - 1) * TEXTURE_SIZE) {
        player->position.x = (SCREEN_COLS - 1) * TEXTURE_SIZE;
    }

    if (*pos_y == 0 && player->position.y < 0) {
        player->position.y = 0;
    } else if (*pos_y == MAP_ROWS - 1 && player->position.y > (SCREEN_ROWS - 1) * TEXTURE_SIZE) {
        player->position.y = (SCREEN_ROWS - 1) * TEXTURE_SIZE;
    }

    if (player->position.y < 0 && *pos_y > 0) {
        *pos_y -= 1;
        player->position.y = (SCREEN_ROWS - 1) * TEXTURE_SIZE;
        find_nearest_walkable_tile(player, overworld, pos_x, pos_y);
    } else if (player->position.y > (SCREEN_ROWS - 1) * TEXTURE_SIZE && *pos_y < MAP_ROWS - 1) {
        *pos_y += 1;
        player->position.y = 0;
        find_nearest_walkable_tile(player, overworld, pos_x, pos_y);
    }

    if (player->position.x < 0 && *pos_x > 0) {
        *pos_x -= 1;
        player->position.x = (SCREEN_COLS - 1) * TEXTURE_SIZE;
        find_nearest_walkable_tile(player, overworld, pos_x, pos_y);
    } else if (player->position.x > (SCREEN_COLS - 1) * TEXTURE_SIZE && *pos_x < MAP_COLS - 1) {
        *pos_x += 1;
        player->position.x = 0;
        find_nearest_walkable_tile(player, overworld, pos_x, pos_y);
    }
}

void render_player(Player *player)
{
    DrawTexture(player->texture, player->position.x, player->position.y, WHITE);
}

void select_player_starting_tile(Player *player, Overworld *overworld, int *pos_x, int *pos_y)
{
    player->position.x = GetRandomValue(0, SCREEN_COLS - 1) * TEXTURE_SIZE;
    player->position.y = GetRandomValue(0, SCREEN_ROWS - 1) * TEXTURE_SIZE;
    if (overworld->screens[*pos_y][*pos_x].tiles[(int)player->position.y / TEXTURE_SIZE][(int)player->position.x / TEXTURE_SIZE] != SAND &&
        overworld->screens[*pos_y][*pos_x].tiles[(int)player->position.y / TEXTURE_SIZE][(int)player->position.x / TEXTURE_SIZE] != GRASS) {
        *pos_x = GetRandomValue(0, MAP_COLS - 1);
        *pos_y = GetRandomValue(0, MAP_ROWS - 1);
        select_player_starting_tile(player, overworld, pos_x, pos_y);
    }
}

Goals_t init_goals(int n)
{
    Goals_t goals = {0, 0, (Vector2) {0, 0}, false, NULL};
    Goals_t *current = &goals;
    for (int i = 0; i < n - 1; i++) {
        current->next = malloc(sizeof(Goals_t));
        current = current->next;
        current->pos_x = rand() % MAP_COLS;
        current->pos_y = rand() % MAP_ROWS;
        current->position = (Vector2) {rand() % SCREEN_COLS * TEXTURE_SIZE, rand() % SCREEN_ROWS * TEXTURE_SIZE};
        current->acquired = false;
        current->next = NULL;
    }
    return goals;
}

void render_goals(Goals_t *goals, Texture2D texture_goals, int pos_x, int pos_y, Player *player)
{
    Goals_t *current = goals;
    while (current != NULL) {
        if (!current->acquired && current->pos_x == pos_x && current->pos_y == pos_y) {
            DrawTexture(texture_goals, current->position.x, current->position.y, WHITE);
            Rectangle playerRec = { player->position.x, player->position.y, TEXTURE_SIZE, TEXTURE_SIZE };
            Rectangle goalRec = { current->position.x, current->position.y, TEXTURE_SIZE, TEXTURE_SIZE };
            printf("Player at (%f, %f)\n", player->position.x, player->position.y);
            printf("Goal at (%f, %f)\n", current->position.x, current->position.y);
            if (CheckCollisionRecs(playerRec, goalRec)) {
                current->acquired = true;
            }
        }
        current = current->next;
    }
}

void print_goals_coordinates(Goals_t *goals)
{
    Goals_t *current = goals;
    while (current != NULL) {
        printf("Goal at (%d, %d)\n", current->pos_x, current->pos_y);
        current = current->next;
    }
}

void display_score(Goals_t *goals)
{
    int score = 0;
    Goals_t *current = goals;
    while (current != NULL) {
        if (current->acquired) {
            score++;
        }
        current = current->next;
    }
    DrawText(TextFormat("Score: %d / 9", score), 10, 10, 20, BLACK);
}

void print_player_position(Player *player, int pos_x, int pos_y)
{
    printf("Player at (%d, %d)\n", pos_x, pos_y);
}

int main() {
    srand(time(NULL));

    generate_map();

    Overworld *overworld = malloc_overworld();
    fill_overworld(overworld);

    int pos_x = overworld->pos_x;
    int pos_y = overworld->pos_y;

    Goals_t goals = init_goals(9);

    InitWindow(SCREEN_COLS * TEXTURE_SIZE, SCREEN_ROWS * TEXTURE_SIZE, "Overworld");

    Player *player = init_player();
    load_textures();

    select_player_starting_tile(player, overworld, &pos_x, &pos_y);

    Texture2D texture_goals = LoadTexture("game/assets/goals.png");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (IsKeyPressed(KEY_N)) {
            generate_map();
            fill_overworld(overworld);
            pos_x = overworld->pos_x;
            pos_y = overworld->pos_y;
            select_player_starting_tile(player, overworld, &pos_x, &pos_y);
            goals = init_goals(9);
        }

        if (IsKeyPressed(KEY_R)) {
            pos_x = rand() % MAP_COLS;
            pos_y = rand() % MAP_ROWS;
            select_player_starting_tile(player, overworld, &pos_x, &pos_y);
        }

        move_player(player, overworld, &pos_x, &pos_y);

        render_screen(overworld, pos_x, pos_y);

        render_goals(&goals, texture_goals, pos_x, pos_y, player);

        print_goals_coordinates(&goals);

        print_player_position(player, pos_x, pos_y);

        render_player(player);

        display_score(&goals);

        EndDrawing();
    }

    for (int i = 0; i < 5; i++) {
        UnloadTexture(textures[i]);
    }

    return 0;
}
