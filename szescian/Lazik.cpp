#include "Lazik.h"
#include "Core.h"
#include "Wheel.h"

Lazik::Lazik(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = zSize;
    this->zSize = ySize;
}

void Lazik::draw(int x, int y, int z)
{
    Core core(x, y, z);
    Wheel wheel(x, y, z);
    core.draw(xSize, ySize, zSize);
    wheel.draw(-15, -5, 13);
    wheel.draw(-15, -5, -13);
    wheel.draw(15, -5, 13);
    wheel.draw(15, -5, -13);
}