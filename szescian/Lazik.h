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
using namespace std;
class HitWall;
class Lazik
{
	int xSize;
	int ySize;
	int zSize;
public:
	Lazik(int xSize, int ySize, int zSize);
	void draw(int x, int y, int z, int wheelRot, float angle);
	//bool isCollision(std::vector<HitWall*> walls, int posX, int posZ);
};

