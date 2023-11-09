#include "Core.h"

Core::Core(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = ySize;
    this->zSize = zSize;
}

void Core::draw(int x, int y, int z)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    {
        GLfloat halfX = this->xSize / 2.0f;
        GLfloat halfY = this->ySize / 2.0f;
        GLfloat halfZ = this->zSize / 2.0f;

        GLfloat centerX = x;
        GLfloat centerY = y;
        GLfloat centerZ = z;

        glBegin(GL_TRIANGLE_STRIP); //bottom
        glColor3f(0.2, 0.2, 0.2);
        for (int z1 = centerZ-halfZ; z1 < centerZ - halfZ+this->zSize; z1 += 10) {
            for (int x1 = centerX - halfX; x1 < centerX - halfX+this->xSize; x1 += 10) {
                glVertex3f(x1, y, z1);
                glVertex3f(x1 + 10, y, z1);
                glVertex3f(x1, y, z+z1+10);
                glVertex3f(x1 + 10, y, z1 + 10);
            }
        }
        glEnd();
        glBegin(GL_TRIANGLE_STRIP); //top
        for (int z1 = centerZ - halfZ; z1 < centerZ - halfZ + this->zSize; z1 += 10) {
            for (int x1 = centerX - halfX; x1 < centerX - halfX + this->xSize; x1 += 10) {
                glVertex3f(x1, this->ySize, z1);
                glVertex3f(x1 + 10, this->ySize, z1);
                glVertex3f(x1, this->ySize, z1 + 10);
                glVertex3f(x1 + 10, this->ySize, z1 + 10);
            }
        }
        glEnd();

        glBegin(GL_TRIANGLE_STRIP); //back
        for (int z1 = centerZ - halfZ; z1 < centerZ - halfZ + this->zSize; z1 += 10) {
            for (int y1 = centerY; y1 < centerY + this->ySize; y1 += 10) {
                glVertex3f(halfX, y1, z1);
                glVertex3f(halfX, y1 + 10, z1);
                glVertex3f(halfX, y1, z1+10);
                glVertex3f(halfX, y1 + 10, z1+10);
            }
        }
        glEnd();

        glBegin(GL_TRIANGLE_STRIP); //front
        for (int z1 = centerZ - halfZ; z1 < centerZ - halfZ + this->zSize; z1 += 10) {
            for (int y1 = centerY; y1 < centerY + this->ySize; y1 += 10) {
                glVertex3f( halfX - this->xSize,  y1, z1);
                glVertex3f( halfX - this->xSize,  y1 + 10, z1);
                glVertex3f( halfX - this->xSize,  y1, z1 + 10);
                glVertex3f( halfX - this->xSize,  y1 + 10, z1 + 10);
            }
        }
        glEnd();

        glBegin(GL_TRIANGLE_STRIP); //left
        for (int x1 = centerX + halfX; x1 < centerX + halfX + this->xSize; x1 += 10) {
            for (int y1 = centerY; y1 < centerY + this->ySize; y1 += 10) {
                glVertex3f(x1 - this->xSize, y1, z+halfZ);
                glVertex3f(x1 - this->xSize, y1 + 10, z + halfZ);
                glVertex3f(x1+10 - this->xSize, y1, z + halfZ);
                glVertex3f(x1+10 - this->xSize, y1 + 10, z + halfZ);
            }
        }
        glEnd();

        glBegin(GL_TRIANGLE_STRIP); //right
        for (int x1 = centerX + halfX; x1 < centerX + halfX + this->xSize; x1 += 10) {
            for (int y1 = centerY; y1 < centerY + this->ySize; y1 += 10) {
                glVertex3f(x1 - this->xSize, y1, z + halfZ - this->zSize);
                glVertex3f(x1 - this->xSize, y1 + 10, z + halfZ - this->zSize);
                glVertex3f(x1 + 10 - this->xSize, y1, z + halfZ - this->zSize);
                glVertex3f(x1 + 10 - this->xSize, y1 + 10, z + halfZ - this->zSize);
            }
        }
        glEnd();
    }
}
