#include "Terrain.h"
#include "loader.h"

Terrain::Terrain()
{
    int scale = 40;
    loadObjFile("plate.obj", 0, 1, 0, 0, -20, 0, scale, scale, scale);
    loadObjFile("mountains.obj", 1, 0, 1, 0, -20, 0, scale, scale, scale);
}
