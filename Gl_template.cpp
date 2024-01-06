#define _CRT_SECURE_NO_WARNINGS
#include "szescian/Terrain.h"
#include "szescian/Lazik.h"

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#ifndef _MBCS
#define _MBCS
#endif

#ifdef _UNICODE
#undef _UNICODE
#endif

#ifdef UNICODE
#undef UNICODE
#endif
#endif

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <cmath>
#include "resource.h"
#include <set>
#include <chrono>
#include "libraries/glut.h"

//gl
#define glRGB(x, y, z) glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42
#define GL_PI 3.14159265359
HPALETTE hPalette = NULL;
std::string name = "Kosiarka";
static LPCTSTR lpszAppName = (LPCTSTR)name.c_str();
static HINSTANCE hInstance;
BITMAPINFOHEADER bitmapInfoHeader;
unsigned char* bitmapData;
unsigned int texture[2];

//timer
#define TIMER_ID 1
#define TIMER_COLLISION_COUNT_ID 2
#define COLLISION_COUNT_TIMER_INTERVAL 5000

//keys
bool isWKeyPressed = false;
bool isSKeyPressed = false;
bool dragging = false;
set<int> keysPressed;
POINTS prevMousePos = { 0, 0 };

//camera
double azimuth = GL_PI;
double elevation = GL_PI / 8;
static GLfloat xCamPos;
static GLfloat yCamPos;
static GLfloat zCamPos;
double camDistance = 200;
double const angleJump = GL_PI / 128;
double const radiusJump = 10;
int const renderDistance = 3000;

//light
double lightPos = elevation*0.3;

//collisions
int collisionCount = 0;
float pointDist = 10;
float lazikDist = 30;
vector<POINT> points1;
vector<POINT> points2;
vector<POINT> points3;
vector<POINT> points4;
static auto lastCollisionCheckTime = std::chrono::high_resolution_clock::now();

//lazik
static GLfloat xPos = 0;
static GLfloat yPos = 0;
static GLfloat zPos = 0;
static GLfloat rot = 0;
float speed = 0;
float maxSpeed = 7;
Lazik lazik(50, 20, 10);

//terrain
Terrain terrain;

//functions:
//lazik
void move();
boolean isCollision(POINT coords);
void sortCollisionPoints();
//render
void SetOpenGLStates();
void SetCamera();
void DrawVehicle();
void DrawTerrain();
void DisplayCollisionCount();
void ResetOpenGLStates();
void RenderScene();
void ResetOpenGLStates();

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ChangeSize(GLsizei w, GLsizei h);

//GL setup
void createScene(HDC& hDC, const HWND& hWnd, HGLRC& hRC);
void SetDCPixelFormat(HDC hDC);
HPALETTE GetOpenGLPalette(HDC hDC);
unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
void DrawText(const char* text, GLfloat x, GLfloat y);
GLfloat dist(POINT col, POINT laz);
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

void move() {
	const float acceleration = 0.3f;
	const float deceleration = 0.2f;
	if (isWKeyPressed || isSKeyPressed) {
		if (isWKeyPressed && speed < maxSpeed) {
			speed += acceleration;
		}
		else if (isSKeyPressed && speed > -maxSpeed) {
			speed -= acceleration;
		}
		if (keysPressed.count('D') && !keysPressed.count('A')) {
			azimuth += (speed / 2 * (GL_PI / 180));
			rot -= speed / 2;
		}
		else if (keysPressed.count('A') && !keysPressed.count('D')) {
			azimuth -= (speed / 2 * (GL_PI / 180));
			rot += speed / 2;
		}
	}
	else {
		if (speed > 0) {
			speed -= deceleration;
		}
		else if (speed < 0) {
			speed += deceleration;
		}
		if (abs(speed) > 0.1) {

			if (keysPressed.count('D') && !keysPressed.count('A')) {
				azimuth += (speed / 2 * 1.0 * (GL_PI / 180));
				rot -= speed / 2 * 1.0f;
			}
			else if (keysPressed.count('A') && !keysPressed.count('D')) {
				azimuth -= (speed / 2 * 1.0 * (GL_PI / 180));
				rot += speed / 2 * 1.0f;
			}
		}
		else {
			speed = 0;
		}
	}
	GLfloat nextX = xPos + speed * sin(rot * (GL_PI / 180) + GL_PI / 2);
	GLfloat nextY = zPos + speed * cos(rot * (GL_PI / 180) + GL_PI / 2);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCollisionCheckTime).count();
	if (elapsedTime >= 10) {
		POINT next{ nextX, nextY };
		if (!isCollision(next)) {
			xPos = nextX;
			zPos = nextY;
		}
		else {
			collisionCount--;
			speed = 0;
		}
		lastCollisionCheckTime = currentTime;
	}
}

boolean isCollision(POINT coords) {
	if (coords.y < 0) {
		if (coords.x > 0) {
			for (auto point : points1) {
				if (dist(point, coords) < pointDist + lazikDist) {
					return true;
				}
			}
		}
		else {
			for (auto point : points2) {
				if (dist(point, coords) < pointDist + lazikDist) {
					return true;
				}
			}
		}
	}
	else {
		if (coords.x > 0) {
			for (auto point : points3) {
				if (dist(point, coords) < pointDist + lazikDist) {
					return true;
				}
			}
		}
		else {
			for (auto point : points4) {
				if (dist(point, coords) < pointDist + lazikDist) {
					return true;
				}
			}
		}
	}
	return false;
}

void sortCollisionPoints()
{
	for (int i = 0; i < terrain.treesAmount; i++) {
		POINT tree{ terrain.randTreeX[i], terrain.randTreeZ[i] };
		if (tree.y < 0) {
			if (tree.x > 0) {
				points1.push_back(tree);
			}
			else {
				points2.push_back(tree);
			}
		}
		else {
			if (tree.x > 0) {
				points3.push_back(tree);
			}
			else {
				points4.push_back(tree);
			}
		}
	}
	for (int i = 0; i <= 125; i += 25) {
		POINT p{ i, 95 };
		if (p.y < 0) {
			if (p.x > 0) {
				points1.push_back(p);
			}
			else {
				points2.push_back(p);
			}
		}
		else {
			if (p.x > 0) {
				points3.push_back(p);
			}
			else {
				points4.push_back(p);
			}
		}
	}
}

void SetOpenGLStates() {

	glNormal3f(0, 1, 0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.04, 0, 0.16, 1.0f);
	glColor3f(0.8f, 0.8f, 0.8f);

	GLfloat lightDir[] = { xPos, yPos, zPos, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);

	GLfloat spotCutoff = 60.0;
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 30);
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &spotCutoff);

	GLfloat spotDirection[] = {
		static_cast<GLfloat>(sin((rot + 90) * GL_PI / 180) * cos(lightPos)),
		static_cast<GLfloat>(sin(lightPos)),
		static_cast<GLfloat>(cos((rot + 90) * GL_PI / 180) * cos(lightPos))
	};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);

	GLfloat increasedAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, increasedAmbient);
	GLfloat increasedDiffuse[] = { 1.2f, 1.2f, 1.2f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, increasedDiffuse);
	GLfloat increasedSpecular[] = { 1.0f, 1.0f, 1.0f, 3.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, increasedSpecular);
}

void SetCamera() {
	glLoadIdentity();
	double a = camDistance * cos(elevation);
	xCamPos = xPos + a * cos(azimuth);
	yCamPos = camDistance * sin(elevation);
	zCamPos = zPos + a * sin(azimuth);
	gluLookAt(xCamPos, yCamPos, zCamPos, xPos, yPos, zPos, 0, 1, 0);
}

void DrawVehicle() {
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(rot, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_BACK, GL_LINE);
	lazik.draw(0, 0, 0, 360 * (speed / maxSpeed), speed);
	glPopMatrix();
}

void DrawTerrain() {
	glPushMatrix();
	glPolygonMode(GL_BACK, GL_LINE);
	terrain.draw();
	glPopMatrix();
}

void DisplayCollisionCount() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	char collisionCountText[100];
	sprintf(collisionCountText, "Punkty: %d", collisionCount);
	DrawText(collisionCountText, 20, 20);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void ResetOpenGLStates() {
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);
	glFlush();
}

void RenderScene() {
	SetOpenGLStates();
	SetCamera();
	DrawVehicle();
	DrawTerrain();
	DisplayCollisionCount();
	ResetOpenGLStates();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	static HGLRC hRC; 
	static HDC hDC;
	switch (message)
	{
		case WM_CREATE:
		{
			createScene(hDC, hWnd, hRC);
			break;
		}
		case WM_DESTROY:
		{
			wglMakeCurrent(hDC, NULL);
			wglDeleteContext(hRC);
			if (hPalette != NULL)
				DeleteObject(hPalette);
			PostQuitMessage(0);
			break;
		}
		case WM_SIZE:
		{
			ChangeSize(LOWORD(lParam), HIWORD(lParam));
			break;
		}
		case WM_PAINT:
		{
			RenderScene();
			SwapBuffers(hDC);
			ValidateRect(hWnd, NULL);
			break;
		}
		case WM_QUERYNEWPALETTE:
		{
			if (hPalette)
			{
				int nRet;
				SelectPalette(hDC, hPalette, FALSE);
				nRet = RealizePalette(hDC);
				InvalidateRect(hWnd, NULL, FALSE);
				return nRet;
			}
			break;
		}
		case WM_PALETTECHANGED:
		{
			if ((hPalette != NULL) && ((HWND)wParam != hWnd))
			{
				SelectPalette(hDC, hPalette, FALSE);
				RealizePalette(hDC);
				UpdateColors(hDC);
				return 0;
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (delta > 0 && camDistance > 150) {
				camDistance -= radiusJump;
			}
			else if (delta < 0 && camDistance < 500) {
				camDistance += radiusJump;
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			prevMousePos = MAKEPOINTS(lParam);
			dragging = true;
			break;
		}
		case WM_RBUTTONUP:
		{
			dragging = false;
			break;
		}
		case WM_TIMER:
		{
			if (wParam == TIMER_ID) {
				move();
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (wParam == TIMER_COLLISION_COUNT_ID) {
				collisionCount += 10;
			}
			break;
		}
		case WM_KEYDOWN:
		{
			keysPressed.insert(wParam);
			if (wParam == 'W') isWKeyPressed = true;
			if (wParam == 'S') isSKeyPressed = true;
			switch (wParam){
				case VK_UP:
				{
					lightPos += 2 * GL_PI / 180;
					break;
				}
				case VK_DOWN:
				{
					lightPos -= 2 * GL_PI / 180;
					break;
				}
			}
			move();
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		case WM_KEYUP:
		{
			keysPressed.erase(wParam);
			if (wParam == 'W') isWKeyPressed = false;
			if (wParam == 'S') isSKeyPressed = false;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (dragging) {
				POINTS currentMousePos = MAKEPOINTS(lParam);
				int deltaX = currentMousePos.x - prevMousePos.x;
				azimuth += angleJump * deltaX / 5;
				prevMousePos = currentMousePos;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		}
		default: return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (0L);
}

void ChangeSize(GLsizei w, GLsizei h) {
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), 0.1f, renderDistance);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void createScene(HDC& hDC, const HWND& hWnd, HGLRC& hRC)
{
	hDC = GetDC(hWnd);
	SetDCPixelFormat(hDC);
	hPalette = GetOpenGLPalette(hDC);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	//
	lazik.init();
	terrain.init();

	SetTimer(hWnd, TIMER_ID, 16, NULL);
	SetTimer(hWnd, TIMER_COLLISION_COUNT_ID, COLLISION_COUNT_TIMER_INTERVAL, NULL);
	glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			
	bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
		bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
	if (bitmapData)
		free(bitmapData);
	bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
		bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
	if (bitmapData)
		free(bitmapData);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	sortCollisionPoints();
}

void SetDCPixelFormat(HDC hDC) {
	int nPixelFormat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,0,
		0,0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

HPALETTE GetOpenGLPalette(HDC hDC) {
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;
	int nColors;
	BYTE RedRange, GreenRange, BlueRange;
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if (!(pfd.dwFlags & PFD_NEED_PALETTE)) return NULL;
	nColors = 1 << pfd.cColorBits;
	LOGPALETTE* pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY));
	pPal->palVersion = 0x300;
	pPal->palNumEntries = nColors;
	RedRange = (1 << pfd.cRedBits) - 1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) - 1;
	for (int i = 0; i < nColors; i++) {
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)((double)pPal->palPalEntry[i].peRed * 255.0 / RedRange);
		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)((double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);
		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)((double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);
		pPal->palPalEntry[i].peFlags = (unsigned char)NULL;
	}
	HPALETTE hRetPal = CreatePalette(pPal);
	SelectPalette(hDC, hRetPal, FALSE);
	RealizePalette(hDC);
	free(pPal);
	return hRetPal;
}

unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader) {
	FILE* filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	BITMAPFILEHEADER bitmapFileHeader;
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fclose(filePtr);
		return NULL;
	}
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	unsigned char* bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);
	if (!bitmapImage) {
		fclose(filePtr);
		return NULL;
	}
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	fclose(filePtr);
	if (!bitmapImage) return NULL;
	for (int imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		unsigned char tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	return bitmapImage;
}

void DrawText(const char* text, GLfloat x, GLfloat y) {
	glRasterPos2f(x, y);
	while (*text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
		++text;
	}
}

GLfloat dist(POINT col, POINT laz) {
	return sqrt(pow((col.x - laz.x), 2) + pow((col.y - laz.y), 2));
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { 
	MSG msg;
	WNDCLASS wc;
	hInstance = hInst;
	wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = lpszAppName;
	if (RegisterClass(&wc) == 0) return FALSE;
	HWND hWnd = CreateWindow(lpszAppName, lpszAppName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 50, 50, 400, 400, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) return FALSE;
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}