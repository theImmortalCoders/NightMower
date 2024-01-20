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
#define glRGB(x, y, z) glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42

using namespace std;
using namespace irrklang;

class Game
{
	HPALETTE hPalette = NULL;
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char* bitmapData;
	unsigned int texture[2];
	ISoundEngine* SoundEngine = createIrrKlangDevice();
	chrono::high_resolution_clock::time_point startTime;
	chrono::high_resolution_clock::time_point lastCollisionCheckTime = std::chrono::high_resolution_clock::now();

	int gameTimeSeconds = 0;
	bool isWKeyPressed = false;
	bool isSKeyPressed = false;
	bool isShiftKeyPressed = false;
	bool dragging = false;
	set<int> keysPressed;
	POINTS prevMousePos = { 0, 0 };
	Lazik lazik;
	Terrain terrain;
	Camera camera;
	bool pause = false;
	bool isHelp = false;
	bool isCollision = false;
	int points = 0;
	int level = 1;

	void createScene(HDC& hDC, const HWND& hWnd, HGLRC& hRC);
	void renderScene();
	void updateTime();
	void updateCamera();
	void drawDashboard();
	void checkCollisions();
	void disableLight();
	void ChangeSize(GLsizei w, GLsizei h);

	void DrawText(const char* text, GLfloat x, GLfloat y);
	void initBitmap(HDC& hDC, const HWND& hWnd, HGLRC& hRC);
	void SetDCPixelFormat(HDC hDC);
	unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader);

public:
	LPCWSTR lpszAppName;
	const int screenX = 1700;
	const int screenY = 900;

	Game();
	void setStartTime(chrono::high_resolution_clock::time_point time);
	LRESULT CALLBACK Game::eventListener(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam);
};