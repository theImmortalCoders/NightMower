#pragma once
#include "loader.h"
#include <Potato.h>
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

	int randomSelect(int min, int max);
public:
	static const int beginPotatoesAmount = 10;
	static const int treesAmount = 40;
	static int potatoesAmount;

	const int scale = 80;
	const float minX = -1000;
	const float maxX = 1000;
	const float minZ = -1000;
	const float maxZ = 1000;

	ObjectData trees[treesAmount];
	ObjectData leaves1[treesAmount];
	ObjectData leaves2[treesAmount];
	std::vector<int> randTreeX;
	std::vector<int> randTreeZ;
	std::vector<Potato> potatoes;

	Wall wall;

	Terrain();
	void load();
	void loadPotatoes();
	void draw();
};