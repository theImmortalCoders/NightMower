#pragma once
#include "loader.h"
#include <Wheel.h>
#include <Handle.h>
#include <Camera.h>
#include <set>

#define GL_PI 3.14159265359

using namespace std;

class Lazik
{
	int xSize;
	int ySize;
	int zSize;
	Handle handle;
	ObjectData core;
	Wheel wheels[4];
	ObjectData reflectors[4];
	void updateLight();
	void drawAxle(int x, int y, int z);
public:
	GLfloat xPos = 0;
	GLfloat yPos = 0;
	GLfloat zPos = 0;
	GLfloat rot = 0;
	float speed = 0;
	float maxSpeed = 4;
	float wheelAngle = 0;
	double lightPos = GL_PI / 12 * 0.3;
	static const int collisionDistance = 25;
	Lazik(int xSize, int ySize, int zSize);
	void move(bool pause, bool& isWKeyPressed, bool& isSKeyPressed, Camera* camera, set<int>& keysPressed);
	void load();
	void draw();
};

