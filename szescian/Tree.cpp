#include <Terrain.h>
#include "Tree.h"
#include <chrono>

Tree::Tree()
{
    x = randomSelect(Terrain::minX, Terrain::maxX);
    z = randomSelect(Terrain::minZ, Terrain::maxZ);
    if (z < 0) {
        if (x > 0) {
            sphereID = 1;
        }
        else {
            sphereID = 2;
        }
    }
    else {
        if (x > 0) {
            sphereID = 3;
        }
        else {
            sphereID = 4;
        }
    }
}

void Tree::load()
{
    wood = loadFile("tree3.obj", "bark.jpg");
    leaves = loadFile("leaves.obj", "bark.jpg");
}

void Tree::draw()
{
    drawObj(&wood, x, -10, z, Terrain::scale, Terrain::scale / 2, Terrain::scale, 1, 1);
    drawObj(&leaves, x, 180, z, Terrain::scale / 3, Terrain::scale / 3, Terrain::scale / 3, 1, 1, 0, 1, 0);
}

////
int Tree::randomSelect(int min, int max)
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