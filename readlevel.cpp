#include "readlevel.h"


WorldMap world_map;

void readlevel(){
    std::ifstream in("level.map");
    int mapWidth, mapHeight; //размер карты
    in >> mapWidth >> mapHeight;

    world_map.resize(mapHeight, std::vector <int> (mapWidth,0));
    for(int i = 0; i < mapHeight; i++) {
        for(int j = 0; j < mapWidth; j++)
            in >> world_map[i][j];
    }
}
/*
*********Краткое русоводство по строению карт************
mapWidth - ширина карты, mapHeight - высота карты. После
сборки карта перевёрнута. Координата игрока - (1,1)
*/
