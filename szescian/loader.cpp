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

void loadObjFile(std::string filename, std::string textureName, GLfloat red, GLfloat green, GLfloat blue, int x, int y, int z, int scaleX, int scaleY, int scaleZ)
{
    std::vector<float> vertices;
    std::vector<float> textures;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    filename = "objects/" + filename;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());
    textureName = "textures/" + textureName;
    GLuint textureID = loadTexture(textureName.c_str());

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (!attrib.texcoords.empty()) {
                textures.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                textures.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            }
        }
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_TRIANGLES);
    glColor3f(red, green, blue);
    int textureIndex = 0;
    for (size_t i = 0; i < vertices.size(); i += 3) {
        if (textureIndex < textures.size()) {
            glTexCoord2f(textures[textureIndex], textures[textureIndex + 1]);
            textureIndex += 2;
        }
        glVertex3f(scaleX * vertices[i] + x, scaleY * vertices[i + 1] + y, scaleZ * vertices[i + 2] + z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &textureID);
}
