#pragma once
#include "glew-2.1.0/include/GL/glew.h"
#include <string>
#include <iostream>
#include <vector>
#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <math.h>
#include <stdio.h>

void loadObjFile(const std::string& filename, const char* textureName, GLfloat red, GLfloat green, GLfloat blue, int x, int y, int z, int sizeX, int sizeY, int sizeZ);