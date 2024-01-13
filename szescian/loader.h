#pragma once
#include "libraries/glew-2.1.0/include/GL/glew.h"
#include <string>
#include <vector>
#include <windows.h>
#include <math.h>

struct ObjectData {
    std::vector<float> vertices;
    std::vector<float> textures;
    GLuint textureID;
};

ObjectData loadFile(const std::string& filename, const std::string& textureName);
void drawObj(ObjectData* objectData, int x, int y, int z, int scaleX, int scaleY, int scaleZ, int repeatX, int repeatY, int r = 1, int g = 1, int b = 1);