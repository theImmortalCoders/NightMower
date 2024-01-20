#include "Lazik.h"
#include "Wheel.h"
#include "Handle.h"
#include <Camera.h>
#include <iostream>

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

void Lazik::draw()
{
    updateLight();
    glPushMatrix();
    glTranslatef(xPos, yPos, zPos);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_BACK, GL_LINE);
    drawObj(&core, 0,  5, 0, xSize * 0.5, zSize, ySize * 0.78, 2, 2);
    handle.draw(-this->xSize + 10, - 10 + 3 * this->ySize, 0, this->zSize);
    drawAxle(4 + this->xSize / 2, 0, 0);
    drawAxle(- 4 - this->xSize / 2, 0, 0);
    wheels[0].draw(  4 + this->xSize / 2, 0,  - this->ySize - 12, 0, speed, wheelAngle);
    wheels[1].draw(  4 + this->xSize / 2, 0,  this->ySize + 12, 1, speed, wheelAngle);
    wheels[2].draw( - 4 - this->xSize / 2, 0,  - this->ySize - 12, 0, speed, 0);
    wheels[3].draw( - 4 - this->xSize / 2, 0,  this->ySize + 12, 1, speed, 0);
    for (int z = -2; z < 2; z++) {
        drawObj(&reflectors[z + 2],  5 + xSize / 2,  3 + ySize, z * 8 + 4, xSize * 0.8, zSize * 0.8, ySize * 0.8, 2, 2);
    }
    glPopMatrix();
}

void Lazik::move(bool pause, bool& isWKeyPressed, bool& isSKeyPressed, bool& isShiftKeyPressed, Camera* camera, set<int>& keysPressed)
{
    if (pause) {
        return;
    }
    if (keysPressed.count(VK_SHIFT)) {
        maxSpeed = 8;
    }
    else {
        if (abs(speed - 8) <2 ) {
            speed = 4;
        }
        maxSpeed = 4;
    }
    const float acceleration = 0.3f;
    const float deceleration = 0.2f;
    wheelAngle = 0;
    float rotJump = speed/3;
    if (isWKeyPressed || isSKeyPressed) {
        if (isWKeyPressed && speed < maxSpeed) {
            speed += acceleration;
        }
        else if (isSKeyPressed && speed > -maxSpeed) {
            speed -= acceleration;
        }
        if (keysPressed.count('D') && !keysPressed.count('A')) {
            camera->azimuth += (rotJump * (GL_PI / 180));
            rot -= rotJump;
            if (speed > 0) {
                wheelAngle = -rotJump*4;
            }
            else {
                wheelAngle = rotJump*4;
            }
        }
        else if (keysPressed.count('A') && !keysPressed.count('D')) {
            camera->azimuth -= (rotJump * (GL_PI / 180));
            rot += rotJump;
            if (speed > 0) {
                wheelAngle = rotJump*4;
            }
            else {
                wheelAngle = -rotJump *4;
            }
        }
    }
    else {
        if (speed > 0) {
            speed -= deceleration;
        }
        else if (speed < 0) {
            speed += deceleration;
        }
        if (abs(speed) > 0.1) {

            if (keysPressed.count('D') && !keysPressed.count('A')) {
                camera->azimuth += (rotJump * (GL_PI / 180));
                rot -= rotJump;
                if (speed > 0) {
                    if (speed > 0) {
                        wheelAngle = -rotJump*4;
                    }
                    else {
                        wheelAngle = rotJump*4;
                    }
                }
            }
            else if (keysPressed.count('A') && !keysPressed.count('D')) {
                camera->azimuth -= (rotJump * (GL_PI / 180));
                rot += rotJump;
                if (speed > 0) {
                    wheelAngle = rotJump *4;
                }
                else {
                    wheelAngle = -rotJump*4;
                }
            }
        }
        else {
            speed = 0;
        }
    }
}

////

void Lazik::updateLight() 
{
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glNormal3f(0, 1, 0);
    glColor3f(0.8f, 0.8f, 0.8f);
    glClearColor(0.0, 0, 0.0, 1.0f);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat lightDir[] = { xPos, yPos, zPos, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    GLfloat spotCutoff = 40.0;
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20);
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &spotCutoff);
    GLfloat spotDirection[] = {
        static_cast<GLfloat>(sin((rot + 90) * GL_PI / 180) * cos(lightPos)),
        static_cast<GLfloat>(sin(lightPos)),
        static_cast<GLfloat>(cos((rot + 90) * GL_PI / 180) * cos(lightPos))
    };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
    GLfloat increasedAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, increasedAmbient);
    GLfloat increasedDiffuse[] = { 1.2f, 1.2f, 1.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, increasedDiffuse);
    GLfloat increasedSpecular[] = { 1.0f, 1.0f, 1.0f, 3.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, increasedSpecular);
}

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