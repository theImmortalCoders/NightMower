#include "Terrain.h"
#include "loader.h"

Terrain::Terrain()
{
    loadObjFile("grass.obj", 0,1,0,0,-10,0,100,100,100);
    loadObjFile("mountains.obj",1,1,0.5,-100,0,0,50,50,50);
}
