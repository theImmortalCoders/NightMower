#pragma once
#ifdef _MSC_VER
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

class Wheel
{
	int x;
	int y;
	int z;
	int rot = 0;
public:
	Wheel(int x, int y, int z);
	void draw(int xSize, int ySize, int zSize, bool mirror = 0, GLfloat rotation = 0, float angle=0);
};

