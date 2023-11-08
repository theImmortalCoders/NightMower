#include "Lazik.h"
#include "Core.h"
#include "Wheel.h"
#include "Axle.h"

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
    Axle axle(x, y, z);
    core.draw(x, y, z);
    axle.draw(x + this->xSize / 2, y, z);
    axle.draw(x - this->xSize / 2, y, z);
    wheel.draw(x+this->xSize/2, 0, x-this->ySize-10);
    wheel.draw(x+this->xSize/2, 0, x+this->ySize+10, 1);
    wheel.draw(x-this->xSize/2, 0, x-this->ySize-10);
    wheel.draw(x-this->xSize/2, 0, x+this->ySize+10, 1);
}