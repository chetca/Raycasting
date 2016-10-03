#ifndef READLEVEL_H
#define READLEVEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <math.h>
#include <fstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TEXTURE_SIZE 64
#define TEXTURE_BLOCK (TEXTURE_SIZE * TEXTURE_SIZE)
/*int mapWidth, mapHeight; //размер карты
void readsize(){
    std::ifstream in("level.map");
    in >> mapWidth >> mapHeight;
}
int world_map[mapWidth][mapHeight];
void readlevel(){
    std::ifstream in("level.map");
    int mapWidth, mapHeight; //размер карты
    in >> mapWidth >> mapHeight;
    int _tmp = 0;
    for(int i = 0; i < mapWidth; i++){
        for(int j = 0; j < mapHeight; j++){
            in >> _tmp;
            world_map[i][j] = _tmp;
        }
    }
}
*/
#define WORLD_SIZE 8
int world_map[WORLD_SIZE][WORLD_SIZE] = {
    { 1, 1, 1, 1, 6, 1, 1, 1 },
    { 1, 0, 0, 1, 0, 0, 0, 7 },
    { 1, 1, 0, 1, 0, 1, 1, 1 },
    { 6, 0, 0, 0, 0, 0, 0, 3 },
    { 1, 8, 8, 0, 8, 0, 8, 1 },
    { 2, 2, 0, 0, 8, 8, 7, 1 },
    { 3, 0, 0, 0, 0, 0, 0, 5 },
    { 2, 2, 2, 2, 7, 4, 4, 4 },
};

#endif // READLEVEL_H
