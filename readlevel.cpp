#include "readlevel.h"

WorldMap world_map;

void readlevel(){
    std::ifstream in("level.map");
    int mapWidth, mapHeight; //размер карты
    in >> mapHeight >> mapWidth;

    world_map.resize(mapHeight);
    for(int i = 0; i < mapHeight; i++) {
        world_map[i].resize(mapWidth);
        for(int j = 0; j < mapWidth; j++)
            in >> world_map[i][j];
    }
}
