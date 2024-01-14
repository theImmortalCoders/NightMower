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
#include "libraries//irrKlang/irrKlang.h"

//music
using namespace irrklang;
ISoundEngine* SoundEngine = createIrrKlangDevice();

//gl
#define glRGB(x, y, z) glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42
#define GL_PI 3.14159265359
HPALETTE hPalette = NULL;
string s = "Nocne koszenie";
std::wstring stemp = std::wstring(s.begin(), s.end());
LPCWSTR lpszAppName = stemp.c_str();
static HINSTANCE hInstance;
BITMAPINFOHEADER bitmapInfoHeader;
unsigned char* bitmapData;
unsigned int texture[2];
const int screenX = 1700;
const int screenY = 900;

//timer
#define TIMER_ID 1
std::chrono::high_resolution_clock::time_point startTime;
int gameTimeSeconds = 0;
bool pause = false;
bool isHelp = false;

//keys
bool isWKeyPressed = false;
bool isSKeyPressed = false;
bool dragging = false;
set<int> keysPressed;
POINTS prevMousePos = { 0, 0 };

//camera
double azimuth = GL_PI;
double elevation = GL_PI / 12;
static GLfloat xCamPos;
static GLfloat yCamPos;
static GLfloat zCamPos;
double camDistance = 200;
double const angleJump = GL_PI / 128;
double const radiusJump = 10;
int const renderDistance = 5000;

//light
double lightPos = elevation*0.3;

//collisions
int collisionCount = 0;
float pointDist = 15;
float lazikDist = 25;
static auto lastCollisionCheckTime = std::chrono::high_resolution_clock::now();
bool isCollision = false;

//lazik
static GLfloat xPos = 0;
static GLfloat yPos = 0;
static GLfloat zPos = 0;
static GLfloat rot = 0;
float speed = 0;
float maxSpeed = 4;
Lazik lazik(40, 20, 10);
float wheelAngle = 0;
int level = 1;

//terrain
Terrain terrain;

//functions:
//lazik
void move();
//render
void updateLight();
void updateCamera();
void drawVehicle();
void drawTerrain();
void drawDashboard();
void disableLight();
void RenderScene();
void updateTime();
void disableLight();

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ChangeSize(GLsizei w, GLsizei h);

//GL setup
void createScene(HDC& hDC, const HWND& hWnd, HGLRC& hRC);
void initBitmap(HDC& hDC, const HWND& hWnd, HGLRC& hRC);
void SetDCPixelFormat(HDC hDC);
unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
void DrawText(const char* text, GLfloat x, GLfloat y);
GLfloat dist(POINT col, POINT laz);
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

void move() {
	if (pause) {
		return;
	}
	const float acceleration = 0.3f;
	const float deceleration = 0.2f;
	wheelAngle = 0;
	if (isWKeyPressed || isSKeyPressed) {
		if (isWKeyPressed) {
			if (speed < maxSpeed) {
				speed += acceleration;
			}
			else speed = maxSpeed;
		}
		else if (isSKeyPressed && speed > -maxSpeed) {
			speed -= acceleration;
		}
		if (keysPressed.count('D') && !keysPressed.count('A')) {
			azimuth += (speed / 3 * (GL_PI / 180));
			rot -= speed / 3;
			if (speed > 0) {
				wheelAngle = -speed*2;
			}
			else {
				wheelAngle = speed*2;
			}
		}
		else if (keysPressed.count('A') && !keysPressed.count('D')) {
			azimuth -= (speed / 3 * (GL_PI / 180));
			rot += speed / 3;
			if (speed > 0) {
				wheelAngle = speed*2;
			}
			else {
				wheelAngle = -speed*2;
			}
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
				azimuth += (speed / 3 * (GL_PI / 180));
				rot -= speed / 3;
				if (speed > 0) {
					if (speed > 0) {
						wheelAngle = -speed*2;
					}
					else {
						wheelAngle = speed*2;
					}
				}
			}
			else if (keysPressed.count('A') && !keysPressed.count('D')) {
				azimuth -= (speed / 3 * (GL_PI / 180));
				rot += speed / 3;
				if (speed > 0) {
					wheelAngle = speed*2;
				}
				else {
					wheelAngle = -speed*2;
				}
			}
		}
		else {
			speed = 0;
		}
	}
	GLfloat nextX = xPos + speed * sin(rot * (GL_PI / 180) + GL_PI / 2);
	GLfloat nextY = zPos + speed * cos(rot * (GL_PI / 180) + GL_PI / 2);
	POINT next{ nextX, nextY };
	auto currentTime = std::chrono::high_resolution_clock::now();
	if (!terrain.checkCollision(next)) {
		xPos = nextX;
		zPos = nextY;
	}
	else {
		if (abs(speed) > 2) {
			collisionCount -= 50;
			isCollision = true;
			lastCollisionCheckTime = currentTime;
			SoundEngine->play2D("audio/explosion.wav", false);
		}
		speed = 0;
	}
	terrain.checkPotatoes(next, collisionCount, SoundEngine, level);
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCollisionCheckTime).count();
	if (elapsedTime > 2000) {
		isCollision = false;
	}
}

void updateLight() {

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glNormal3f(0, 1, 0);
	glColor3f(0.8f, 0.8f, 0.8f);
	glClearColor(0.04, 0, 0.16, 1.0f);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat lightDir[] = { xPos, yPos, zPos, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
	GLfloat spotCutoff = 40.0;
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20);
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

void updateCamera() {
	glLoadIdentity();
	double a = camDistance * cos(elevation);
	xCamPos = xPos + a * cos(azimuth);
	yCamPos = camDistance * sin(elevation);
	zCamPos = zPos + a * sin(azimuth);
	gluLookAt(xCamPos, yCamPos, zCamPos, xPos, yPos, zPos, 0, 1, 0);
}

void drawVehicle() {
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(rot, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_BACK, GL_LINE);
	lazik.draw(0, 0, 0, speed, wheelAngle);
	glPopMatrix();
}

void drawTerrain() {
	glPushMatrix();
	glPolygonMode(GL_BACK, GL_LINE);
	terrain.draw();
	glPopMatrix();
	for (auto& potato : terrain.potatoes) {
		glPushMatrix();
		glPolygonMode(GL_BACK, GL_LINE);
		glTranslatef(potato.x, 2*sin(potato.heightArgument), potato.z);
		glRotatef(potato.angle, 0, 1, 0);
		potato.heightArgument += 0.1;
		if (potato.heightArgument > 1000) potato.heightArgument = 0;
		potato.angle += 1;
		if (potato.angle > 360) potato.angle = 0;
		potato.draw();
		glPopMatrix();
	}
}

void drawDashboard() {
	char collisionCountText[100];
	char levelText[100];
	char left[100];
	char timeText[100];
	char collText[100];
	char pauseText[100];
	char steeringText1[20];
	char steeringText2[50];
	char steeringText3[20];
	char steeringText4[20];
	char steeringText5[20];
	char steeringText6[20];
	char steeringText7[20];
	char helpText[20];
	sprintf(collisionCountText, "Punkty: %d", collisionCount);
	sprintf(levelText, "Poziom %d", level);
	sprintf(left, "Pozostalo %d ziemniakow", terrain.potatoCounter);
	sprintf(timeText, "Czas gry: %d sekund", gameTimeSeconds);
	sprintf(collText, "Kolizja! -50");
	sprintf(steeringText1, "Pauza [ESC]");
	sprintf(steeringText4, "Od nowa [R]");
	sprintf(steeringText2, "Przod [W], tyl [S], lewo [A], prawo [D]");
	sprintf(steeringText3, "Przyspiesz [Shift]");
	sprintf(steeringText5, "Swiatla [UP]/[DOWN]");
	sprintf(steeringText6, "Kamera [PPM + mouse]");
	sprintf(steeringText7, "Reset pozycji [SPACE]");
	sprintf(helpText, "Sterowanie [H]");
	DrawText(timeText, 20, 170);
	DrawText(collisionCountText, 20, 20);
	DrawText(levelText, 20, 70);
	DrawText(left, 20, 120);
	DrawText(helpText, 20, 920);
	if (pause) {
		sprintf(pauseText, "Pauza");
		DrawText(pauseText, 700, 120);
	}
	if (isCollision) {
		DrawText(collText, 700, 20);
	}
	if (isHelp) {
		DrawText(steeringText1, 20, 580);
		DrawText(steeringText4, 20, 540);
		DrawText(steeringText2, 20, 500);
		DrawText(steeringText3, 20, 460);
		DrawText(steeringText5, 20, 420);
		DrawText(steeringText6, 20, 380);
		DrawText(steeringText7, 20, 340);
	}
}

void disableLight() {
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glFlush();
}

void RenderScene() {
	updateTime();
	updateLight();
	updateCamera();
	drawVehicle();
	drawTerrain();
	drawDashboard();
	disableLight();
}

void updateTime()
{
	if (!pause) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
		gameTimeSeconds = static_cast<int>(elapsedTime);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	static HGLRC hRC; 
	static HDC hDC;
	static HWND closeButton;
	static HWND leaderboardButton;
	switch (message)
	{
		case WM_CREATE:
		{
			createScene(hDC, hWnd, hRC);

			break;

			break;
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
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;

			lpMMI->ptMinTrackSize.x = screenX;
			lpMMI->ptMinTrackSize.y = screenY;

			return 0;
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
			else if (delta < 0 && camDistance < 3500) {
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
			break;
		}
		case WM_KEYDOWN:
		{
			keysPressed.insert(wParam);
			if (wParam == 'W') isWKeyPressed = true;
			if (wParam == 'S') isSKeyPressed = true;
			if (wParam == 'H') isHelp = !isHelp;
			if (wParam == VK_SPACE) {
				xPos = 0;
				zPos = 0;
				rot = 0;
				azimuth = GL_PI;
				speed = 0;
			}
			if (wParam == 'R') {
				xPos = 0; 
				zPos = 0;
				rot = 0;
				azimuth = GL_PI;
				speed = 0;
				collisionCount = 0;
				gameTimeSeconds = 0;
				startTime = std::chrono::high_resolution_clock::now();
				level = 1;
				terrain.potatoesAmount = Terrain::beginPotatoesAmount;
				terrain.potatoCounter = terrain.potatoesAmount;
				terrain.loadPotatoes();
			}
			if (wParam == VK_ESCAPE) {
				pause = !pause;
			}
			if (wParam == VK_SHIFT) maxSpeed =8;
			switch (wParam){
				case VK_UP:
				{
					if (lightPos < 12 * GL_PI / 180)
					{
						lightPos += 2 * GL_PI / 180;
					}
					break;
				}
				case VK_DOWN:
				{
					if (lightPos > -8 * GL_PI / 180) {
						lightPos -= 2 * GL_PI / 180;
					}
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
			if (wParam == VK_SHIFT) maxSpeed =4;
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
			}
			break;
		}
		default:
			return (DefWindowProc(hWnd, message, wParam, lParam));
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
	initBitmap(hDC, hWnd, hRC);

	lazik.load();
	terrain.load();
	SoundEngine->play2D("audio/breakout.mp3", true);
	SetTimer(hWnd, TIMER_ID, 16, NULL);
}

void initBitmap(HDC& hDC, const HWND& hWnd, HGLRC& hRC)
{
	hDC = GetDC(hWnd);
	SetDCPixelFormat(hDC);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	glGenTextures(2, &texture[0]);
	bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
		bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
	if (bitmapData)
		free(bitmapData);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i(static_cast<int>(x + 5), static_cast<int>(y + 20));

	for (const char* c = text; *c != '\0'; ++c) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2i(static_cast<int>(x), static_cast<int>(y));
	glVertex2i(static_cast<int>(x) + 200, static_cast<int>(y));
	glVertex2i(static_cast<int>(x) + 200, static_cast<int>(y) + 60);
	glVertex2i(static_cast<int>(x), static_cast<int>(y) + 60);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	startTime = std::chrono::high_resolution_clock::now();
	MSG msg;
	WNDCLASS wc = { 0 };
	hInstance = hInst;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = lpszAppName;
	if (RegisterClass(&wc) == 0) return FALSE;
	HWND hWnd = CreateWindow(lpszAppName, lpszAppName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 50, 50, screenX, screenY, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) return FALSE;
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}