#include "Coords.h"

Coords::Coords(int x, int z)
{
    this->x = x;
    this->z = z;
}

int Coords::getX()
{
    return x;
}

int Coords::getZ()
{
    return z;
}
