#pragma once
#include "loader.h"
#ifdef _MSC_VER
#  pragma comment(lib, "opengl32.lib")
#  pragma comment(lib, "glu32.lib")
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>
#include "Wall.h"
#include <vector>

class Terrain
{
	ObjectData plate;
	ObjectData mountains;
public:
	static const int treesAmount = 20;
	ObjectData trees[treesAmount];
	void init();
	std::vector<int> randTreeX;
	std::vector<int> randTreeZ;
	Wall* wall;
	int scale = 80;
	Terrain();
	~Terrain();
	void draw();
};

