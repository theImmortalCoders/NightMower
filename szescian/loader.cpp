#include "loader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"
#include <gl/GL.h>

void loadObjFile(const std::string& filename, GLfloat red, GLfloat green, GLfloat blue, int x, int y, int z, int scaleX, int scaleY, int scaleZ)
{
    std::vector<float> vertices;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);
        }
    }
    glBegin(GL_TRIANGLES);
    glColor3f(red, green, blue);
    for (size_t i = 0; i < vertices.size(); i += 3) {
        glVertex3f(scaleX * vertices[i] + x, scaleY * (vertices[i + 1]) + y, scaleZ * vertices[i + 2] + z);
    }
    glEnd();
}