#pragma once
#ifdef _MSC_VER
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

class Axle
{
private:
	int x;
	int y;
	int z;
	GLfloat camDistance = 1;
	GLfloat length = 15;
public:
	Axle(int x, int y, int z);
	void draw(int x, int y, int z);
};