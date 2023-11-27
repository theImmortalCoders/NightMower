#pragma once
#include <string>
#include <iostream>
#include <vector>
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

void loadObjFile(const std::string& filename, GLfloat red, GLfloat green, GLfloat blue, int x, int y, int z, int sizeX, int sizeY, int sizeZ);