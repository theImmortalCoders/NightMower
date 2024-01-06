#include "Wheel.h"
#include <cmath>
#include <thread>

GLfloat PI = 3.14159;
GLfloat deg90 = PI / 2;
GLfloat deg9 = PI / 20;
GLfloat deg1 = deg90 / 90;
//width probably shouldnt be higher than 10
const int width = 6; //must be even value


void tire() {
    GLfloat bend = 10 * deg1;
    int stripes = width / 2;   // num of stripes in a half a tire
    int outRds = 12; // outer radius
    int innRds = 7;  // inner radius

    //-----outer part of a tire-----
    glColor3f(0, 0, 0);
    for (int i = -stripes; i < stripes; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        GLfloat a1 = cos(bend * i);
        GLfloat a2 = cos(bend * (i + 1));
        for (int j = 0; j < 41; j++) {
            GLfloat b = deg9 * j;

            GLfloat x = 2 * i;
            GLfloat y = outRds * a1 * cos(b);
            GLfloat z = outRds * a1 * sin(b);
            glVertex3d(x, y, z);
            x = 2 * (i + 1);
            y = outRds * a2 * cos(b);
            z = outRds * a2 * sin(b);
            glVertex3d(x, y, z);
        }
        glEnd();
    }

    //-----inner part of a tire-----
    for (int i = -stripes; i < stripes; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j < 41; j++) {

            GLfloat x = 2 * i;
            GLfloat y = innRds * cos(deg9 * j);
            GLfloat z = innRds * sin(deg9 * j);
            glVertex3d(x, y, z);
            x = 2 * (i + 1);
            glVertex3d(x, y, z);
        }
        glEnd();
    }

    //-----side of a tire-----
    glColor3f(0.8, 0, 0);
    for (int k = 0;k < 2;k++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < 41; i++) {
            GLfloat a = deg9 * i;

            GLfloat x = k ? width : -width;
            GLfloat y = innRds * sin(a);
            GLfloat z = innRds * cos(a);
            glVertex3d(x, y, z);
            y = outRds * sin(a) * cos(bend * stripes);
            z = outRds * cos(a) * cos(bend * stripes);
            glVertex3d(x, y, z);
        }
        glEnd();
    }
}

void spokes() { // szprycha i guess
    for (int rot = 0;rot < 6;rot++) {
        glRotatef(60, 1.0, 0, 0);
        for (int k = 0; k < 2; k++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < 7; i++) {
                GLfloat x = width - 0.5 - k;
                GLfloat y = i + 2.4;
                GLfloat z = 1;
                glVertex3d(x, y, z);
                glVertex3d(x, y, -z);
            }
            glEnd();
            glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < 7; i++) {
                GLfloat x = width - 2.5;
                GLfloat y = i + 2.9;
                GLfloat z = k ? -0.5 : 0.5;
                glVertex3d(x, y, z);
                x += 2;
                glVertex3d(x, y, z);
            }
            glEnd();
        }
    }
}

void rim(bool flip, GLfloat someDegree) {

    glScalef(flip?-1:1,1,1);

    //-----male kolko-----
    //glColor3f(0.65, 0.65, 0.65);
    glColor3f(0.8, 0.8, 0.8);
    for (int k = 0; k < 2; k++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < 41; i++) {
            GLfloat a = deg9 * i;
            glVertex3d(width - 0.5 -k, sin(a), cos(a));
            glVertex3d(width - 0.5 -k, 3 * sin(a), 3 * cos(a));
        }
        glEnd();
    }
    for (int k = 0;k < 2;k++) {
        for (int j = 0; j < 41; j++) {
            glBegin(GL_TRIANGLE_STRIP);
            GLfloat x = width - 1.5;
            GLfloat y = (k?3:1) * cos(deg9 * j);
            GLfloat z = (k?3:1) * sin(deg9 * j);
            glVertex3d(x, y, z);
            x = width - 0.5;
            glVertex3d(x, y, z);
        }
        glEnd();
    }
    //-----szprychy czy jakos tak-----
    glRotatef(someDegree, 1, 0, 0);
    spokes();
}

Wheel::Wheel(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

int randoma(int min, int max)
{
    static bool first = true;
    if (first)
    {
        srand(time(NULL));
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

void Wheel::draw(int x, int y, int z, bool mirror, GLfloat rotation, float angle)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    {
        glPushMatrix();
        glTranslatef(x,  y,  z);
        glRotatef(90, 0, 1.0, 0);
        glRotatef(angle, 0, 1, 0);

        tire();
        rot += rotation * randoma(90, 180);
        rim(mirror, rot);

        glPopMatrix();
    }
}