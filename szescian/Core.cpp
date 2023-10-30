#include "Core.h"

Core::Core(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Core::draw(int x, int y, int z)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    {
        GLfloat halfX = x / 2.0f;
        GLfloat halfY = y / 2.0f;
        GLfloat halfZ = z / 2.0f;

        GLfloat centerX = this->x;
        GLfloat centerY = this->y;
        GLfloat centerZ = this->z;

        glBegin(GL_POLYGON);
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
        glEnd();
    }
}
