#include "Wheel.h"


Wheel::Wheel(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Wheel::draw(int x, int y, int z)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    {
        GLfloat halfX = x / 2.0f;
        GLfloat halfY = y / 2.0f;
        GLfloat halfZ = z / 2.0f;

        GLfloat centerX = this->x;
        GLfloat centerY = this->y;
        GLfloat centerZ = this->z;
        glPushMatrix();
        glTranslatef(centerX + x, centerY + y, centerZ + z);
        glColor3f(0,0,0);
        GLUquadricObj* obj;
        obj = gluNewQuadric();
        gluQuadricTexture(obj, GL_TRUE);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glEnable(GL_TEXTURE_2D);
        gluSphere(obj, 10, 15, 7);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}