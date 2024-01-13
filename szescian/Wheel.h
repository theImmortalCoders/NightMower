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
	int rot = 0;
	const GLfloat PI = 3.14159;
	const GLfloat deg90 = PI / 2;
	const GLfloat deg9 = PI / 20;
	const GLfloat deg1 = deg90 / 90;
	const int width = 6;
	void drawTire();
	void drawSpokes();
	void drawRim(bool flip, GLfloat someDegree);
public:
	void draw(int xSize, int ySize, int zSize, bool mirror = 0, GLfloat rotation = 0, float angle=0);
};

