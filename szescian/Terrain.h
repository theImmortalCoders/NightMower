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
	static const int treesAmount = 50;
	static const int potatoesAmount = 20;
	ObjectData trees[treesAmount];
	ObjectData leaves1[treesAmount];
	ObjectData leaves2[treesAmount];
	ObjectData potatoes[potatoesAmount];
	void init();
	std::vector<int> randTreeX;
	std::vector<int> randTreeZ;
	std::vector<int> randPotatoeX;
	std::vector<int> randPotatoeY;
	Wall* wall;
	int scale = 80;
	Terrain();
	~Terrain();
	void draw();
	void drawPotatoe(int x, int y, int i);
};
int random(int min, int max);
