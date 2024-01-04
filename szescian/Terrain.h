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
#include "Wall.h"
#include <vector>
class HitWall;
class Terrain
{
private:
	std::vector<HitWall*> hitWalls;
public:
	std::vector<int> randTreeX;
	std::vector<int> randTreeZ;
	Wall* wall;

	int scale = 80;
	Terrain();
	~Terrain();
	void draw();
	void generateRandomTrees();
	std::vector<HitWall*> getHitWalls();
};

