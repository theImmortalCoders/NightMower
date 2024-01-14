#include "Lazik.h"
#include "Wheel.h"
#include "Handle.h"

Lazik::Lazik(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = zSize;
    this->zSize = ySize;
}

void Lazik::load()
{
    core = loadFile("core.obj", "floor.jpg");
    for (int i = 0; i < 4; i++) {
        reflectors[i] = loadFile("reflector.obj", "metal.jpg");
    }
}

void Lazik::draw(int x, int y, int z, int wheelRot, float angle)
{
    drawObj(&core, x, y + 5, z, xSize * 0.5, zSize, ySize * 0.78, 2, 2);
    handle.draw(x - this->xSize + 10, y - 10 + 3 * this->ySize, z, this->zSize);
    drawAxle(x + 4 + this->xSize / 2, y, z);
    drawAxle(x - 4 - this->xSize / 2, y, z);
    wheels[0].draw(x + 4 + this->xSize / 2, 0, x - this->ySize - 12, 0, wheelRot, angle);
    wheels[1].draw(x + 4 + this->xSize / 2, 0, x + this->ySize + 12, 1, wheelRot, angle);
    wheels[2].draw(x - 4 - this->xSize / 2, 0, x - this->ySize - 12, 0, wheelRot, 0);
    wheels[3].draw(x - 4 - this->xSize / 2, 0, x + this->ySize + 12, 1, wheelRot, 0);
    for (int z = -2; z < 2; z++) {
        drawObj(&reflectors[z + 2], x + 5 + xSize / 2, y + 3 + ySize, z * 8 + 4, xSize * 0.8, zSize * 0.8, ySize * 0.8, 2, 2);
    }
}

////
void Lazik::drawAxle(int x, int y, int z)
{
    GLfloat PI = 3.14159;
    GLfloat deg9 = PI / 20;
    GLfloat camDistance = 1;
    GLfloat length = 15;

    glPushMatrix();
    glTranslatef(x, y, z);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    {
        glColor3f(0, 0, 0);
        for (int i = -length; i < length; i++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < 41; j++) {
                GLfloat x = 2 * i;
                GLfloat y = camDistance * cos(deg9 * j);
                GLfloat z = camDistance * sin(deg9 * j);
                glVertex3d(z, y, x);
                x = 2 * (i + 1);
                glVertex3d(z, y, x);
            }
            glEnd();
        }
        for (int k = 0; k < 2; k++) {
            int capDist = (k ? -2 : 2) * length;
            glBegin(GL_TRIANGLE_FAN);
            for (int j = 0; j < 41; j++) {
                GLfloat x = cos(deg9 * j);
                GLfloat y = sin(deg9 * j);
                GLfloat z = capDist;
                glVertex3d(x, y, z);
            }
            glEnd();
        }
    }
    glPopMatrix();
}