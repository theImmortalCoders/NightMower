#pragma once
#include "loader.h"
#include <Wheel.h>
#include <Handle.h>

using namespace std;
class Lazik
{
	int xSize;
	int ySize;
	int zSize;
	Handle handle;
	ObjectData core;
	Wheel wheels[4];
	ObjectData reflectors[4];
	void drawAxle(int x, int y, int z);
public:
	Lazik(int xSize, int ySize, int zSize);
	void load();
	void draw(int x, int y, int z, int wheelRot, float angle);
};

