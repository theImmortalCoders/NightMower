#pragma once
#ifdef _MSC_VER
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

class Wall
{
	int x;
	int y;
	int z;
	int height;
	int width;
public:
	Wall(int x, int y, int z, int width, int height);
	void draw();
};

