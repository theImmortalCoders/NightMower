#include "loader.h"
#include "Lazik.h"
#include "Wheel.h"
#include "Axle.h"
#include "Handle.h"
#include <string>
#include <iostream>
#include <vector>
#include "Wall.h"
#include "HitWall.h"
#include <math.h>
#define GL_PI 3.14159265359
Lazik::Lazik(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = zSize;
    this->zSize = ySize;
}

void Lazik::draw(int x, int y, int z, int wheelRot, float angle)

{
    loadObjFile("core.obj", "floor.jpg", 1, 1, 1, x, y+5, z, xSize*0.5, zSize, ySize*0.78, 2, 2);
    Wheel wheel(x, y, z);
    Axle axle(x, y, z);
    Handle handle(x, y, z);
    handle.draw(x - this->xSize + 10, y - 10 + 3 * this->ySize, z, this->zSize);
    axle.draw(x + 4 + this->xSize / 2, y, z);
    axle.draw(x - 4 - this->xSize / 2, y, z);
    wheel.draw(x + 4 + this->xSize / 2, 0, x - this->ySize - 12, 0, wheelRot, angle);
    wheel.draw(x + 4 + this->xSize / 2, 0, x + this->ySize + 12, 1, wheelRot, angle);
    wheel.draw(x - 4 - this->xSize / 2, 0, x - this->ySize - 12, 0, wheelRot);
    wheel.draw(x - 4 - this->xSize / 2, 0, x + this->ySize + 12, 1, wheelRot);
    for (int z = -2; z < 2; z++) {
        loadObjFile("reflector.obj", "metal.jpg", 1, 1, 1, x + 5 + xSize / 2, y + 3 + ySize, z * 8 + 4, xSize*0.8, zSize*0.8, ySize*0.8, 2, 2);
    }
}

bool Lazik::isCollision(std::vector<HitWall*> walls, int posX, int posZ)
{
    vector<Coords*> points;
    points.push_back(new Coords(posX + this->xSize / 2, posZ));
    points.push_back(new Coords(posX - this->xSize / 2, posZ));
    points.push_back(new Coords(posX, posZ - this->zSize/2));
    points.push_back(new Coords(posX, posZ + this->zSize/2));
    for (auto wall : walls) {
        for (auto point : points) {
            float startX = wall->beginning.getX();
            float startZ = wall->beginning.getZ();
            float endX = wall->end.getX();
            float endZ = wall->end.getZ();
            for (int i = 0; i < 5; i++) {
                float t = i / 4;
                float x = (1 - t) * startX + t * endX;
                float z = (1 - t) * startZ + t * endZ;
                float distance = sqrt(pow(point->getX() - x, 2) + pow(point->getZ() - z, 2));
                if (distance < 0.5) {
                    return true;
                }
            }
        }
    }
    return false;
}
