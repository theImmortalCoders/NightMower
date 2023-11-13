#include "Lazik.h"
#include "Core.h"
#include "Wheel.h"
#include "Axle.h"
#include "Handle.h"
#include "Reflector.h"
#include <string>
#include <iostream>
#include <vector>

#include "loader.h"

Lazik::Lazik(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = zSize; // zamiana z Y na Z ¿eby by³ nie na boku
    this->zSize = ySize;
}

void Lazik::draw(int x, int y, int z)
{
    Core core(this->xSize * 1.2, this->ySize, this->zSize * 1.5);
    Wheel wheel(x, y, z);
    Axle axle(x, y, z);
    Handle handle(x, y, z);
    Reflector reflector(x, y, z);

    handle.draw(x - this->xSize + 10, y - 10 + 3 * this->ySize, z, this->zSize);
    core.draw(x, y, z);
    axle.draw(x + 4 + this->xSize / 2, y, z);
    axle.draw(x - 4 - this->xSize / 2, y, z);
    axle.draw(x, y, z);
    wheel.draw(x, 0, x - this->ySize - 12);
    wheel.draw(x, 0, x + this->ySize + 12, 1);
    wheel.draw(x + 4 + this->xSize / 2, 0, x - this->ySize - 12);
    wheel.draw(x + 4 + this->xSize / 2, 0, x + this->ySize + 12, 1);
    wheel.draw(x - 4 - this->xSize / 2, 0, x - this->ySize - 12);
    wheel.draw(x - 4 - this->xSize / 2, 0, x + this->ySize + 12, 1);
    for (int z = -2; z < 2; z++) {
        reflector.draw(x + 5 + this->xSize / 2, y + 3 + this->ySize, z * 8 + 4);
    }
    this->cow();
    std::vector<float> vertices;
    loadObjFile("grass.obj", vertices);
    glBegin(GL_TRIANGLES);

    for (size_t i = 0; i < vertices.size(); i += 3) {
        glVertex3f( 10* vertices[i], 10* (vertices[i + 1])-50,10* vertices[i + 2]);
    }
    glEnd();
}

void Lazik::cow()
{
    std::vector<float> vertices;
    loadObjFile("untitled.obj", vertices);
    glBegin(GL_TRIANGLES);
    glColor3f(1.000, 0.078, 0.576);
    for (size_t i = 0; i < vertices.size(); i += 3) {
        glVertex3f(5 * vertices[i], 5 * (vertices[i + 1]) + 27, 5 * vertices[i + 2]);
    }
    glEnd();
}
