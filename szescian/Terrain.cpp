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

    // Define the range for x and z values
    const float minX = -250.0f;
    const float maxX = 250;
    const float minZ = -250;
    const float maxZ = 250;

    // Loop to generate random trees
    for (int i = 0; i < treesAmount; ++i) {
        // Generate random values for x and z within the specified range
        float randomX = random(minX, maxX);
        float randomZ = random(minZ, maxZ);
        this->randTreeX.push_back(randomX);
        this->randTreeZ.push_back(randomZ);
       // this->trees[i] = loadObjFile("tree3.obj", "bark.jpg");
    }
    //this->plate = loadObjFile("plate.obj", "grass.jpg");
    //this->mountains = loadObjFile("mountains.obj", "rock.jpg");
}

void Terrain::draw()
{
    loadObjFile("plate.obj", "grass.jpg", 1, 1, 1, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    loadObjFile("mountains.obj", "rock.jpg", 1, 1, 1, 0, -20, 0, scale, scale / 2, scale, 10, 10);
    //trees
    for (int i = 0; i < 10; i++) {
        int randomX = randTreeX[i];
        int randomZ = randTreeZ[i];
        loadObjFile("tree3.obj", "bark.jpg", 1, 1, 1, randomX, -10, randomZ, scale, scale / 2, scale, 5, 5);
        //loadObjFile("leaves.obj", "", 0, 0.75, 0, randomX, 150, randomX / 2, scale, scale, scale / 2, 1, 1);
        //loadObjFile("leaves.obj", "", 0, 0.7, 0, randomX, 150, randomX * 2, scale * 0.7, scale / 2 * 0.7, scale * 0.7, 1, 1);
    }
    /* loadObjFile("tree3.obj", "bark.jpg", 1, 1, 1, 160, -10, 40, scale, scale/2, scale, 5, 5);
     loadObjFile("leaves.obj", "", 0, 0.75, 0, 160, 150, 20, scale, scale, scale/2, 1, 1);
     loadObjFile("leaves.obj", "", 0, 0.7, 0, 160, 150, 80, scale * 0.7, scale/2 * 0.7, scale * 0.7, 1, 1);*/
     //wall->draw();
}

Terrain::~Terrain()
{
    delete wall;
}