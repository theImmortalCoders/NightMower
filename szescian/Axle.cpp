#include "axle.h"

Axle::Axle(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = 1;
    this->length = 15;
}

void Axle::draw(int x, int y, int z) {
    GLfloat PI = 3.14159;
    GLfloat deg9 = PI / 20;

    glPushMatrix();
    glTranslatef(x, y, z);

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