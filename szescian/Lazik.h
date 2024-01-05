#pragma once
#include "loader.h"
#ifdef _MSC_VER
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <vector>

using namespace std;
class Lazik
{
	int xSize;
	int ySize;
	int zSize;
	ObjectData core;
	ObjectData reflectors[4];
public:
	Lazik(int xSize, int ySize, int zSize);
	void init();
	void draw(int x, int y, int z, int wheelRot, float angle);
};

