#include "loader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "libraries/tinyobjloader/tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libraries/stb_image.h"

GLuint loadTexture(const char* filename) {
    GLuint texture;
    glewInit();
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, STBI_rgb);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    return texture;
}

ObjectData loadFile(const std::string& filename, const std::string& textureName) {
    ObjectData objData;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    std::string objFilePath = "objects/" + filename;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objFilePath.c_str());
    std::string textureFilePath = "textures/" + textureName;
    objData.textureID = loadTexture(textureFilePath.c_str());
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            objData.vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            objData.vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            objData.vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (!attrib.texcoords.empty()) {
                objData.textures.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                objData.textures.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            }
        }
    }

    return objData;
}

void drawObj(ObjectData* objectData, int x, int y, int z, int scaleX, int scaleY, int scaleZ, int repeatX, int repeatY, int r, int g, int b) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, objectData->textureID);
    glBegin(GL_TRIANGLES);
    int textureIndex = 0;
    glColor3f(r, g, b);
    for (size_t i = 0; i < objectData->vertices.size(); i += 3) {
        if (textureIndex < objectData->textures.size()) {
            glTexCoord2f(repeatX * objectData->textures[textureIndex], repeatY * objectData->textures[textureIndex + 1]);
            textureIndex += 2;
        }
        glVertex3f(scaleX * objectData->vertices[i] + x, scaleY * objectData->vertices[i + 1] + y, scaleZ * objectData->vertices[i + 2] + z);
    }

    glEnd();
    glDisable(GL_TEXTURE_2D);
}