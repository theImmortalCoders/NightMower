#include "Reflector.h"

Reflector::Reflector(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = 3;
    this->length = 2;
}

void Reflector::draw(int x, int y, int z) {
    GLfloat PI = 3.14159;
    GLfloat deg9 = PI / 20;

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-90, 0, 1, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    {
        glColor3f(0, 0, 0);
        for (int i = -length; i < length; i++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < 41; j++) {
                GLfloat x = 2 * i;
                GLfloat y = radius * cos(deg9 * j);
                GLfloat z = radius * sin(deg9 * j);
                glVertex3d(z, y, x);
                x = 2 * (i + 1);
                glVertex3d(z, y, x);
            }
            glEnd();
        }
        for (int k = 0;k < 2; k++) {
            int capDist = (k ? -2 : 2) * length;
            if (k) glColor3f(1, 1, 0);
            glBegin(GL_TRIANGLE_FAN);
            for (int j = 0; j < 41; j++) {
                GLfloat x = radius * cos(deg9 * j);
                GLfloat y = radius * sin(deg9 * j);
                GLfloat z = capDist;
                glVertex3d(x, y, z);
            }
            glEnd();
        }
    }
    glPopMatrix();
}