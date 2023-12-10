#pragma once
#include "libraries/glew-2.1.0/include/GL/glew.h"
#include <string>
#include <iostream>
#include <vector>
#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <math.h>
#include <stdio.h>

void loadObjFile(std::string filename, std::string textureName, GLfloat red, GLfloat green, GLfloat blue, int x, int y, int z, int scaleX, int scaleY, int scaleZ, int repeatX, int repeatY);