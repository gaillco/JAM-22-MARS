#include "../include/generator.h"

int main()
{
    generate_map();

    Image mapImage = GenImageColor(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE, RAYWHITE);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            Rectangle dest = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            switch (map[i][j]) {
                case GRASS:
                    ImageDrawRectangleRec(&mapImage, dest, GREEN);
                    break;
                case SAND:
                    ImageDrawRectangleRec(&mapImage, dest, YELLOW);
                    break;
                case WATER:
                    ImageDrawRectangleRec(&mapImage, dest, BLUE);
                    break;
                case OBSTACLE_GREEN:
                    ImageDrawRectangleRec(&mapImage, dest, DARKGREEN);
                    break;
                case OBSTACLE_BROWN:
                    ImageDrawRectangleRec(&mapImage, dest, BROWN);
                    break;
            }
        }
    }
    
    ExportImage(mapImage, "map.png");
    UnloadImage(mapImage);

    return 0;
}