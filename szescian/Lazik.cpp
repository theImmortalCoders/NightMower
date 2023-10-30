#include "Lazik.h"
#include "Core.h"
#include "Wheel.h"

Lazik::Lazik(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Lazik::draw(int xSize, int ySize, int zSize)
{
    Core core(this->x, this->y, this->z);
    Wheel wheel(this->x, this->y, this->z);
    core.draw(xSize, ySize, zSize);
    wheel.draw(-15, 13, -5);
    wheel.draw(-15, -13, -5);
    wheel.draw(15, 13, -5);
    wheel.draw(15, -13, -5);
}