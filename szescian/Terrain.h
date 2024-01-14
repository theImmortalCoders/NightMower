#pragma once
#include "loader.h"
#include "Tree.h"
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
#include <libraries/irrKlang/ik_ISoundEngine.h>
using namespace irrklang;
using namespace std;

class Terrain
{
	ObjectData plate;
	ObjectData mountains;

	GLfloat dist(POINT col, POINT laz);
	void sortWallsCollisions();
public:
	static const int collisionPointDistance = 15;
	static const int beginPotatoesAmount = 10;
	static const int treesAmount = 40;
	static int potatoesAmount;
	int potatoCounter = beginPotatoesAmount;

	static const int scale = 80;
	static const int minX = -1000;
	static const int maxX = 1000;
	static const int minZ = -1000;
	static const int maxZ = 1000;

	vector<Tree> trees;
	vector<POINT> points1;
	vector<POINT> points2;
	vector<POINT> points3;
	vector<POINT> points4;
	std::vector<Potato> potatoes;

	Wall wall;

	Terrain();
	void load();
	void loadPotatoes();
	void draw();

	void checkPotatoes(POINT coords, int& points, ISoundEngine* soundEngine, int& level);
	bool checkCollision(POINT coords);

};