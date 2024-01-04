#include "loader.h"
#include "Terrain.h"
#include "Wall.h"
#include <chrono>

int random(int min, int max)
{
    static bool first = true;
    if (first)
    {
        srand(time(NULL));
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}


Terrain::Terrain()
{
    wall = new Wall(30, -10, 20 + 70, 5, 5);
    const float minX = -300;
    const float maxX = 300;
    const float minZ = -300;
    const float maxZ = 300;
    for (int i = 0; i < treesAmount; ++i) {
        float randomX = random(minX, maxX);
        float randomZ = random(minZ, maxZ);
        this->randTreeX.push_back(randomX);
        this->randTreeZ.push_back(randomZ);
    }
}

void Terrain::draw()
{
    loadObjFile("plate.obj", "grass.jpg", 1, 1, 1, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    loadObjFile("mountains.obj", "rock.jpg", 1, 1, 1, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    //trees
    for (int i = 0; i < treesAmount; i++) {
        int randomX = randTreeX[i];
        int randomZ = randTreeZ[i];
        loadObjFile("tree3.obj", "bark.jpg", 1, 1, 1, randomX, -10, randomZ, scale, scale / 2, scale, 5, 5);
    }
    wall->draw();
}

Terrain::~Terrain()
{
    delete wall;
}