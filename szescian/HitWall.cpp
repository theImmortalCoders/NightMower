#include "HitWall.h"
#include <math.h>

HitWall::HitWall(int xB, int zB, int xE, int zE) : beginning(xB, zB), end(xE, zE)
{}

bool HitWall::isHit(Coords point)
{
    float A = (beginning.getZ() - end.getZ()) / (beginning.getX() - end.getX());
    float B = beginning.getZ() - (A * beginning.getX());
    float C = 1;
    float distance = fabsf(A*point.getX()+B*point.getZ()+C)/sqrt(pow(A,2)+pow(B,2));
    return distance < 0.5;
}