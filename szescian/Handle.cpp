#include "handle.h"

Handle::Handle(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = 1;
    this->length = 13;
}

void Handle::draw(int x, int y, int z, int zSize) {
    GLfloat PI = 3.14159;
    GLfloat deg9 = PI / 20;

    glPushMatrix();
    glTranslatef(x, y, z);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    {
        glRotatef(90, 1, 0, 0);
        glRotatef(45, 0, 1, 0);
        glColor3f(0, 0, 0);
        //hand right
        for (int i = -length; i < length; i++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < 41; j++) {
                GLfloat x = 2 * i ;
                GLfloat y = radius * cos(deg9 * j) + 10 - zSize;
                GLfloat z = radius * sin(deg9 * j);
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
                GLfloat y = sin(deg9 * j) + 10 - zSize;
                GLfloat z = capDist;
                glVertex3d(x, y, z);
            }
            glEnd();
        }
        //hand left
        for (int i = -length; i < length; i++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < 41; j++) {
                GLfloat x = 2 * i;
                GLfloat y = radius * cos(deg9 * j) -10 + zSize;
                GLfloat z = radius * sin(deg9 * j);
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
                GLfloat y = sin(deg9 * j) - 10 + zSize;
                GLfloat z = capDist;
                glVertex3d(x, y, z);
            }
            glEnd();
        }
        //bar
        glRotatef(-90, 1, 0, 0);
        glRotatef(-360, 0, 1, 0);
        for (int i = -(length/2)+1; i < (length/2)-2; i++) {
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
        //bar
        glTranslatef(-x, -y, -z);
        glTranslatef(x, y+zSize+5.5, z);
        for (int i = -(length / 2) + 1; i < (length / 2) - 2; i++) {
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
        //bar
        glTranslatef(-x, -y - zSize - 5.5, -z);
        glTranslatef(x, y + zSize - 4, z);
        for (int i = -(length / 2) + 1; i < (length / 2) - 2; i++) {
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
    }

    glPopMatrix();
    
}