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
public:
	static const int beginPotatoesAmount = 10;
	const float minX = -1000;
	const float maxX = 1000;
	const float minZ = -1000;
	const float maxZ = 1000;
	static int potatoesAmount;
	static const int treesAmount = 40;
	std::vector<Potato> potatoes;
	ObjectData trees[treesAmount];
	ObjectData leaves1[treesAmount];
	ObjectData leaves2[treesAmount];
	void init();
	void initPotatoes();
	std::vector<int> randTreeX;
	std::vector<int> randTreeZ;
	Wall* wall;
	int scale = 80;
	Terrain();
	~Terrain();
	void draw();
};
int random(int min, int max);
