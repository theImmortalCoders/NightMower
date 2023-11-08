#include "Lazik.h"
#include "Core.h"
#include "Wheel.h"

Lazik::Lazik(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = zSize; //zamiana z Y na Z ¿eby by³ nie na boku
    this->zSize = ySize;
}

void Lazik::draw(int x, int y, int z)
{
    Core core(this->xSize*1.2, this->ySize*2, this->zSize*1.5);
    Wheel wheel(x, y, z);
    core.draw(x, y, z);
    wheel.draw(x, z, y, 1);
}