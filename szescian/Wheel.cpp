#include "Wheel.h"


Wheel::Wheel(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Wheel::draw(int x, int y, int z, bool flip)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    {
        GLfloat halfX = x / 2.0f;
        GLfloat halfY = y / 2.0f;
        GLfloat halfZ = z / 2.0f;

        GLfloat centerX = this->x;
        GLfloat centerY = this->y;
        GLfloat centerZ = this->z;

        GLfloat PI = 3.14159;
        GLfloat deg = PI / 20;

        int kola[2] = {7, 10};
        int flippedX = 1 + 8 * flip;

        glPushMatrix();
        glTranslatef(centerX + x, centerY + y, centerZ + z);
        glColor3f(0,0,0);

        for each (int var in kola)
        {
            for (int i = 0; i < 5; i++) {
                glBegin(GL_TRIANGLE_STRIP);
                for (int j = 0; j < 41; j++) {
                    glVertex3d(2 * i, var * cos(deg * j), var * sin(deg * j));
                    glVertex3d(2 * (i + 1), var * cos(deg * j), var * sin(deg * j));
                }
                glEnd();
            }
        }
        glColor3f(1, 0, 0);
        for (int k = 0;k < 2;k++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < 41; i++) {
                glVertex3d(10 * k, 7 * sin(deg * i), 7 * cos(deg * i));
                glVertex3d(10 * k, 10 * sin(deg * i), 10 * cos(deg * i));
            }
            glEnd();
        }
        glColor3f(0.65, 0.65, 0.65);
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < 41; i++) {
            glVertex3d(flippedX, sin(deg * i), cos(deg * i));
            glVertex3d(flippedX, 3 * sin(deg * i), 3 * cos(deg * i));
        }
        glEnd();
        for (int rot = 0;rot < 3;rot++) {
            glRotatef(60, 1.0, 0, 0);
            for (int k = 0;k < 2;k++) {
                glBegin(GL_TRIANGLE_STRIP);
                for (int i = 0; i < 6; i++) {
                    glVertex3d(flippedX, i + 2 - (10 * k), 1.4);
                    glVertex3d(flippedX, i + 2 - (10 * k), -1.4);
                }
                glEnd();
            }
        }
        glPopMatrix();
    }
}