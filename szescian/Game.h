#pragma once
#include <Lazik.h>
#include <Terrain.h>
#include <Camera.h>
#include <chrono>
#include <set>
#include "libraries//irrKlang/irrKlang.h"
#include <windef.h>
#include <libraries/glut.h>

#define TIMER_ID 1
#define GL_PI 3.14159265359

using namespace std;
using namespace irrklang;

class Game
{
	//sound
	ISoundEngine* SoundEngine = createIrrKlangDevice();
	//time
	chrono::high_resolution_clock::time_point startTime;
	chrono::high_resolution_clock::time_point lastCollisionCheckTime = std::chrono::high_resolution_clock::now();
	int gameTimeSeconds = 0;
	//keys
	bool isWKeyPressed = false;
	bool isSKeyPressed = false;
	bool dragging = false;
	set<int> keysPressed;
	POINTS prevMousePos = { 0, 0 };
	//light
	double lightPos = GL_PI / 12 * 0.3;
	//game
	Lazik lazik;
	Terrain terrain;
	Camera camera;
	bool pause = false;
	bool isHelp = false;
	bool isCollision = false;
	int points = 0;
	int level = 1;

	//lazik?
	GLfloat xPos = 0;
	GLfloat yPos = 0;
	GLfloat zPos = 0;
	GLfloat rot = 0;
	float speed = 0;
	float maxSpeed = 4;
	float wheelAngle = 0;
public:
	//screen
	const int screenX = 1700;
	const int screenY = 900;

	Game();
	void updateCamera();
	void drawDashboard();
	void DrawText(const char* text, GLfloat x, GLfloat y);
	void disableLight();
	void renderScene();
	void updateTime();
	void setStartTime(chrono::high_resolution_clock::time_point time);
	void ChangeSize(GLsizei w, GLsizei h);
	LRESULT CALLBACK Game::eventListener(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam);

	//gl?
	#define glRGB(x, y, z) glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
	#define BITMAP_ID 0x4D42
	HPALETTE hPalette = NULL;
	string s = "Nocne koszenie";
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR lpszAppName = stemp.c_str();
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char* bitmapData;
	unsigned int texture[2];

	void createScene(HDC& hDC, const HWND& hWnd, HGLRC& hRC);
	void initBitmap(HDC& hDC, const HWND& hWnd, HGLRC& hRC);
	void SetDCPixelFormat(HDC hDC);
	unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader);

	//Terrain?
	void drawTerrain();

	//lazik?
	void move();
	void updateLight();
	void drawVehicle();
};

