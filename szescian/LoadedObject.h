#pragma once
#include "libraries/glew-2.1.0/include/GL/glew.h"
#include <vector>
class LoadedObject {
public:
    GLuint textureID;
    std::vector<float> vertices;
    std::vector<float> textures;
};