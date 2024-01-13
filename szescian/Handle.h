#pragma once
#ifdef _MSC_VER
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

class Handle
{
    const GLfloat PI = 3.14159;
    const GLfloat deg9 = PI / 20;
    const GLfloat camDistance = 1;
    const GLfloat length = 13;
public:
	void draw(int x, int y, int z, int zSize);
};

