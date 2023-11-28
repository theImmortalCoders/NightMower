#include "loader.h"
#include "Terrain.h"
#include "Wall.h"

Terrain::Terrain()
{
    int scale = 40;
    loadObjFile("plate.obj", 0, 1, 0, 0, -20, 0, scale, scale, scale);
    loadObjFile("mountains.obj", 1, 0, 1, 0, -20, 0, scale, scale, scale);
    loadObjFile("tree3.obj", 0.2, 0, 0, 160, -10, 40, scale, scale, scale);
    loadObjFile("leaves.obj", 0, 0.75, 0, 160, 150, 20, scale, scale, scale);
    loadObjFile("leaves.obj", 0, 0.7, 0, 160, 150, 80, scale * 0.7, scale * 0.7, scale * 0.7);
    Wall wall(30,0, 20);
    wall.draw(30, 0, 20 + 70, 5, 5);
}
