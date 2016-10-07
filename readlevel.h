#ifndef READLEVEL_H
#define READLEVEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <math.h>
#include <fstream>
#include <vector>

typedef std::vector<std::vector<int> > WorldMap;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TEXTURE_SIZE 64
#define TEXTURE_BLOCK (TEXTURE_SIZE * TEXTURE_SIZE)

extern WorldMap world_map;

void readlevel();

#endif // READLEVEL_H
