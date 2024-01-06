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
    int liczba = min + rand() % ((max + 1) - min);
    while (abs(liczba) < 30) {
        liczba = min + rand() % ((max + 1) - min);
    }
    return liczba;
}

void Terrain::init() {
    plate = loadFile("plate.obj", "grass.jpg");
    mountains = loadFile("mountains.obj", "rock.jpg");
    for (int i = 0; i < treesAmount; i++) {
        trees[i] = loadFile("tree3.obj", "bark.jpg");
        leaves1[i] = loadFile("leaves.obj", "bark.jpg");
        leaves2[i] = loadFile("leaves.obj", "bark.jpg"); 
    }
}

Terrain::Terrain()
{
    wall = new Wall(30, -10, 20 + 70, 5, 5);
    const float minX = -500;
    const float maxX = 500;
    const float minZ = -500;
    const float maxZ = 500;
    for (int i = 0; i < treesAmount; ++i) {
        float randomX = random(minX, maxX);
        float randomZ = random(minZ, maxZ);
        this->randTreeX.push_back(randomX);
        this->randTreeZ.push_back(randomZ);
    }
}

void Terrain::draw()
{
    drawObj(&plate, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    drawObj(&mountains, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    //trees
    for (int i = 0; i < treesAmount; i++) {
        int randomX = randTreeX[i];
        int randomZ = randTreeZ[i];
        drawObj(&trees[i], randomX, -10, randomZ, scale, scale / 2, scale, 1, 1);
        drawObj(&leaves1[i], randomX, 180, randomZ, scale/3, scale/3, scale / 3, 1, 1, 0, 1, 0);
    }
    wall->draw();
}

Terrain::~Terrain()
{
    delete wall;
}