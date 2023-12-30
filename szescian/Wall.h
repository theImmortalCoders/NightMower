#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#ifdef _MSC_VER
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>
#include <stdio.h>
#include <vector>
class HitWall;
class Wall
{
	int x;
	int y;
	int z;
	int height;
	int width;
	//std::vector<HitWall*> hitWalls;
public:
	Wall(int x, int y, int z, int width, int height);
	//std::vector<HitWall*>getHitWalls();
	void draw();
	std::pair<std::pair<int, int>, std::pair<int, int>> Wall::getWallCoordinates();
};

