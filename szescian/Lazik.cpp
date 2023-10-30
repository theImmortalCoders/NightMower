#include "Lazik.h"
#include "Core.h"

Lazik::Lazik(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Lazik::draw(int xSize, int ySize, int zSize)
{
    Core core(this->x, this->y, this->z);
    core.draw(xSize, ySize, zSize);
}