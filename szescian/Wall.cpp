#include "Wall.h"
#include "HitWall.h"
#include <iostream>
void cegla(int x, int z) {
	x -= 30;
	z -= 30;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int k = 0; k < 2; k++) {
		for (int j = 0; j < 12; j++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(0.9, 0.1, 0.1);

			for (int i = 0; i < 6; i++)
			{
				glVertex3d(2 * j + x, 2 * i, -6 * k + z);
				glVertex3d(2 * (j + 1) + x, 2 * i, -6 * k + z);
			}
			glEnd();
		}
	}
	for (int k = 0; k < 2; k++) {
		for (int j = 0; j < 12; j++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(0.7, 0.1, 0.1);

			for (int i = 0; i < 4; i++)
			{
				glVertex3d(2 * j + x, 10 * k, -2 * i + z);
				glVertex3d(2 * (j + 1) + x, 10 * k, -2 * i + z);
			}
			glEnd();
		}
	}
	for (int k = 0; k < 2; k++) {
		for (int j = 0; j < 5; j++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(0.8, 0.1, 0.1);

			for (int i = 0; i < 4; i++)
			{
				glVertex3d(24 * k + x, 2 * j, -2 * i + z);
				glVertex3d(24 * k + x, 2 * (j + 1), -2 * i + z);
			}
			glEnd();
		}
	}
}

Wall::Wall(int x, int y, int z, int width, int height)
{
    this->x = x;
    this->y = y;
    this->z = z;
	this->height = height;
	this->width = width;
	this->hitWalls.push_back(new HitWall(x - width / 2, z, x + width / 2, z));
}

std::vector<HitWall*> Wall::getHitWalls()
{
	std::cout << "xd";
	return this->hitWalls;
}

void Wall::draw()
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(90,1,0,0);
	for (int j = 0;j < height;j++)
		for (int i = 0;i < width + j % 2;i++)
			cegla(25 * i - j % 2 * 13, 7 * j);
	glPopMatrix();
}
