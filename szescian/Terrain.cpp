#include "Terrain.h"
#include "Wall.h"
#include <chrono>
int Terrain::potatoesAmount = Terrain::beginPotatoesAmount;
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
    initPotatoes();
}

Terrain::Terrain()
{
    wall = new Wall(30, -10, 20 + 70, 5, 5);
    for (int i = 0; i < treesAmount; ++i) {
        float randomX = random(minX, maxX);
        float randomZ = random(minZ, maxZ);
        this->randTreeX.push_back(randomX);
        this->randTreeZ.push_back(randomZ);
    }
}

void Terrain::initPotatoes()
{
    potatoes.clear();
    for (int i = 0; i < potatoesAmount; i++) {
        Potato potato;
        potato.object = loadFile("potato.obj", "potato.jpg");
        potato.x = random(minX + 100, maxX - 100);
        potato.z = random(minZ + 100, maxZ - 100);
        potato.angle = random(0, 360);
        potato.heightArgument = random(0, 200) / 100.0;
        potatoes.push_back(potato);
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