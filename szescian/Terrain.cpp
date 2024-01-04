#include "loader.h"
#include "Terrain.h"
#include "Wall.h"
#include "HitWall.h"
Terrain::Terrain()
{
    wall = new Wall(30, -10, 20 + 70, 5, 5);
    /*for (auto h : wall->getHitWalls()) {
        this->hitWalls.push_back(h);
    }*/
}

void Terrain::draw()
{
    loadObjFile("plate.obj", "grass.jpg", 1, 1, 1, 0, -20, 0, scale, scale/2, scale, 10, 10);
    loadObjFile("mountains.obj", "rock.jpg", 1, 1, 1, 0, -20, 0, scale, scale/2, scale, 10, 10);
    loadObjFile("tree3.obj", "bark.jpg", 1, 1, 1, 160, -10, 40, scale, scale/2, scale, 5, 5);
    loadObjFile("leaves.obj", "", 0, 0.75, 0, 160, 150, 20, scale, scale, scale/2, 1, 1);
    loadObjFile("leaves.obj", "", 0, 0.7, 0, 160, 150, 80, scale * 0.7, scale/2 * 0.7, scale * 0.7, 1, 1);
    wall->draw();
}

Terrain::~Terrain()
{
    delete wall;
}

//std::vector<HitWall*> Terrain::getHitWalls()
//{
//    return this->hitWalls;
//}
