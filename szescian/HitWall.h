#pragma once
#include "Coords.h"
class HitWall
{
public:
	Coords beginning;
	Coords end;
	HitWall(int xB, int zB, int xE, int zE);
	bool isHit(Coords point);
};

