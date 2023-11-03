#include "Core.h"

Core::Core(int xSize, int ySize, int zSize)
{
    this->xSize = xSize;
    this->ySize = ySize;
    this->zSize = zSize;
}

void Core::draw(int x, int y, int z)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    {
        GLfloat halfX = this->xSize / 2.0f;
        GLfloat halfY = this->ySize / 2.0f;
        GLfloat halfZ = this->zSize / 2.0f;

        GLfloat centerX = x;
        GLfloat centerY = y;
        GLfloat centerZ = z;

        glBegin(GL_TRIANGLE_STRIP); //bottom
        glColor3f(0.0f, 1.0f, 0.0f);
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
        glColor3f(0.0f, 1.0f, 0.0f);
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
        glColor3f(2.0f,1.0f, 0.0f);
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
        glColor3f(0.0f, 1.0f, 1.0f);
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
        glColor3f(0.0f, 1.0f, 0.0f);
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
        glColor3f(0.0f, 1.0f, 0.0f);
        for (int x1 = centerX + halfX; x1 < centerX + halfX + this->xSize; x1 += 10) {
            for (int y1 = centerY; y1 < centerY + this->ySize; y1 += 10) {
                glVertex3f(x1 - this->xSize, y1, z + halfZ - this->zSize);
                glVertex3f(x1 - this->xSize, y1 + 10, z + halfZ - this->zSize);
                glVertex3f(x1 + 10 - this->xSize, y1, z + halfZ - this->zSize);
                glVertex3f(x1 + 10 - this->xSize, y1 + 10, z + halfZ - this->zSize);
            }
        }
        glEnd();

        /*glBegin(GL_POLYGON);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-halfX + centerX, -halfY + centerY, -halfZ + centerZ);
        glVertex3f(halfX + centerX, -halfY + centerY, -halfZ + centerZ);
        glVertex3f(halfX + centerX, -halfY + centerY, halfZ + centerZ);
        glVertex3f(-halfX + centerX, -halfY + centerY, halfZ + centerZ);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(halfX + centerX, -halfY + centerY, -halfZ + centerZ);
        glVertex3f(halfX + centerX, halfY + centerY, -halfZ + centerZ);
        glVertex3f(halfX + centerX, halfY + centerY, halfZ + centerZ);
        glVertex3f(halfX + centerX, -halfY + centerY, halfZ + centerZ);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(halfX + centerX, halfY + centerY, -halfZ + centerZ);
        glVertex3f(-halfX + centerX, halfY + centerY, -halfZ + centerZ);
        glVertex3f(-halfX + centerX, halfY + centerY, halfZ + centerZ);
        glVertex3f(halfX + centerX, halfY + centerY, halfZ + centerZ);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-halfX + centerX, halfY + centerY, -halfZ + centerZ);
        glVertex3f(-halfX + centerX, -halfY + centerY, -halfZ + centerZ);
        glVertex3f(-halfX + centerX, -halfY + centerY, halfZ + centerZ);
        glVertex3f(-halfX + centerX, halfY + centerY, halfZ + centerZ);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(-halfX + centerX, halfY + centerY, halfZ + centerZ);
        glVertex3f(-halfX + centerX, -halfY + centerY, halfZ + centerZ);
        glVertex3f(halfX + centerX, -halfY + centerY, halfZ + centerZ);
        glVertex3f(halfX + centerX, halfY + centerY, halfZ + centerZ);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-halfX + centerX, -halfY + centerY, -halfZ + centerZ);
        glVertex3f(halfX + centerX, -halfY + centerY, -halfZ + centerZ);
        glVertex3f(halfX + centerX, halfY + centerY, -halfZ + centerZ);
        glVertex3f(-halfX + centerX, halfY + centerY, -halfZ + centerZ);
        glEnd();*/
    }
}
