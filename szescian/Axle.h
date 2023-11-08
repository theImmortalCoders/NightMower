#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#ifdef _MSC_VER
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>
#include <stdio.h>

class Axle
{
private:
	int x;
	int y;
	int z;
	GLfloat radius;
	GLfloat length;
public:
	Axle(int x, int y, int z);
	void draw(int x, int y, int z);
};