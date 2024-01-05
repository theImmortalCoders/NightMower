#include "Lazik.h"
#include "Wheel.h"
#include "Axle.h"
#include "Handle.h"

Lazik::Lazik(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = zSize;
    this->zSize = ySize;
}

void Lazik::init()
{
    core = loadFile("core.obj", "floor.jpg");
    for (int i = 0; i < 4; i++) {
        reflectors[i] = loadFile("reflector.obj", "metal.jpg");
    }
}

void Lazik::draw(int x, int y, int z, int wheelRot, float angle)

{
    drawObj(&core, x, y + 5, z, xSize * 0.5, zSize, ySize * 0.78, 2, 2);
    Wheel wheel(x, y, z);
    Axle axle(x, y, z);
    Handle handle(x, y, z);
    handle.draw(x - this->xSize + 10, y - 10 + 3 * this->ySize, z, this->zSize);
    axle.draw(x + 4 + this->xSize / 2, y, z);
    axle.draw(x - 4 - this->xSize / 2, y, z);
    wheel.draw(x + 4 + this->xSize / 2, 0, x - this->ySize - 12, 0, angle = angle);
    wheel.draw(x + 4 + this->xSize / 2, 0, x + this->ySize + 12, 1, angle = angle);
    wheel.draw(x - 4 - this->xSize / 2, 0, x - this->ySize - 12, 0);
    wheel.draw(x - 4 - this->xSize / 2, 0, x + this->ySize + 12, 1);
    for (int z = -2; z < 2; z++) {
        drawObj(&reflectors[z + 2], x + 5 + xSize / 2, y + 3 + ySize, z * 8 + 4, xSize * 0.8, zSize * 0.8, ySize * 0.8, 2, 2);
    }
}