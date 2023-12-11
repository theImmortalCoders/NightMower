#include "loader.h"
#include "Lazik.h"
#include "Wheel.h"
#include "Axle.h"
#include "Handle.h"
#include <string>
#include <iostream>
#include <vector>
#include "Wall.h"
#define GL_PI 3.14159265359
Lazik::Lazik(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = zSize;
    this->zSize = ySize;
}

void Lazik::draw(int x, int y, int z, int wheelRot)
{
    loadObjFile("core.obj", "floor.jpg", 1, 1, 1, x, y+5, z, xSize*0.5, zSize, ySize*0.78, 2, 2);
    Wheel wheel(x, y, z);
    Axle axle(x, y, z);
    Handle handle(x, y, z);
    handle.draw(x - this->xSize + 10, y - 10 + 3 * this->ySize, z, this->zSize);
    axle.draw(x + 4 + this->xSize / 2, y, z);
    axle.draw(x - 4 - this->xSize / 2, y, z);
    wheel.draw(x + 4 + this->xSize / 2, 0, x - this->ySize - 12, 0, wheelRot);
    wheel.draw(x + 4 + this->xSize / 2, 0, x + this->ySize + 12, 1, wheelRot);
    wheel.draw(x - 4 - this->xSize / 2, 0, x - this->ySize - 12, 0, wheelRot);
    wheel.draw(x - 4 - this->xSize / 2, 0, x + this->ySize + 12, 1, wheelRot);
    for (int z = -2; z < 2; z++) {
        loadObjFile("reflector.obj", "metal.jpg", 1, 1, 1, x + 5 + xSize / 2, y + 3 + ySize, z * 8 + 4, xSize*0.8, zSize*0.8, ySize*0.8, 2, 2);
    }
}