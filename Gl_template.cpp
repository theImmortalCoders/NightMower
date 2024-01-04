#define  _CRT_SECURE_NO_WARNINGS
#ifdef _MSC_VER                         // Check if MS Visual C compiler
#  pragma comment(lib, "opengl32.lib")  // Compiler-specific directive to avoid manually configuration
#  pragma comment(lib, "glu32.lib")     // Link libraries
#endif
#ifdef _MSC_VER        // Check if MS Visual C compiler
#   ifndef _MBCS
#      define _MBCS    // Uses Multi-byte character set
#   endif
#   ifdef _UNICODE     // Not using Unicode character set
#      undef _UNICODE 
#   endif
#   ifdef UNICODE
#      undef UNICODE 
#   endif
#endif
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include "resource.h"           // About box resource identifiers.
#include "szescian/Lazik.h"
#include "szescian/Terrain.h"
#include <set>
#include <chrono>
#include <iostream>
#include "libraries/glut.h"
#define TIMER_ID 1
#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14159265359

HPALETTE hPalette = NULL;
static LPCTSTR lpszAppName = "Kosiarka";
static HINSTANCE hInstance;

//keys
bool isWKeyPressed = false;
bool isSKeyPressed = false;
set<int> keysPressed;

bool dragging = false;
POINTS prevMousePos = { 0, 0 };
//camera
double azimuth = GL_PI;   // K¹t poziomy
double elevation = GL_PI/8; // K¹t pionowy

static GLfloat xCamPos;
static GLfloat yCamPos;
static GLfloat zCamPos;
//collisions
float pointDist = 10;
float lazikDist = 30;
//lazik
static GLfloat xPos = 0;
static GLfloat yPos = 0;
static GLfloat zPos = 0;
static GLfloat rot = 0;
float speed = 0;
float maxSpeed = 7;
double camDistance = 200;
double const angleJump = GL_PI / 128;
double const radiusJump = 10;
Lazik lazik(50, 20, 10);
Terrain terrain;

#define TIMER_COLLISION_COUNT_ID 2

// Add the following line after the #define TIMER_ID 1 line
#define COLLISION_COUNT_TIMER_INTERVAL 5000 // 10 seconds

// Add the following line after the SetTimer(hWnd, TIMER_ID, 16, NULL); line in the WM_CREATE case


BITMAPINFOHEADER	bitmapInfoHeader;	// nag³ówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury

LRESULT CALLBACK WndProc(   HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam);
INT_PTR APIENTRY AboutDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void SetDCPixelFormat(HDC hDC);

GLfloat dist(POINT col, POINT laz) {
	return sqrt(pow((col.x - laz.x), 2) + pow((col.y - laz.y), 2));
}
boolean isCollision(POINT coords) {
	vector<POINT> points1;
	vector<POINT> points2;
	POINT tree{ 160, 40 };
	points1.push_back(tree);
	//
	for (int i = 10; i <= 135; i += 25) {
		POINT p{ i, 95 };
		points2.push_back(p);
	}
	//
	if (coords.y < 45) {
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
	return false;
}

void ChangeSize(GLsizei w, GLsizei h) {
	GLfloat fAspect;
	GLfloat fFrustumScale = 1.0;
	if (h == 0)
		h = 1;
	static GLsizei lastHeight;
	static GLsizei lastWidth;
	lastWidth = w;
	lastHeight = h;
	fAspect = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, fAspect, 0.1f, 1500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void DrawText(const char* text, GLfloat x, GLfloat y) {
	glRasterPos2f(x, y);

	while (*text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
		++text;
	}
}

void SetupRC() {
    glEnable(GL_DEPTH_TEST); // Hidden surface removal
    glEnable(GL_LIGHTING);   // Enable lighting
    glEnable(GL_LIGHT0);     // Enable light source 0
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    // Set up light source 0 position
    GLfloat lightPos[] = { 100.0f, 100.0f, 100.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    // Set up light source 0 properties
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0, 0.0, 0.0);
}


unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader){
	FILE *filePtr;							// wskaŸnik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag³ówek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;		// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany sk³adowych
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	fclose(filePtr);
	return bitmapImage;
}
int collisionCount = 0;
void RenderScene(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glLoadIdentity();
	double a = camDistance * cos(elevation);
	xCamPos = xPos + a * cos(azimuth);
	yCamPos = camDistance * sin(elevation);
	zCamPos = zPos + a * sin(azimuth);
	gluLookAt(xCamPos, yCamPos, zCamPos, xPos, yPos, zPos, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(rot, 0.0f, 1.0f, 0.0f);

	glPolygonMode(GL_BACK, GL_LINE);
	float rotWheel = speed / 2;

	lazik.draw(0, 0, 0, 360 * (speed / maxSpeed), rotWheel);
	glPopMatrix();
	glPushMatrix();
	glPolygonMode(GL_BACK, GL_LINE);
	GLfloat lightPos[] = { 1000.0f, 1000.0f, 1000.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	terrain.draw();
	glPopMatrix();
	glFlush();

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0, 0.0, 0.0); // Kolor tekstu (czerwony)
	char collisionCountText[50];
	sprintf(collisionCountText, "Punkty: %d", collisionCount);
	DrawText(collisionCountText, 10, 10);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void SetDCPixelFormat(HDC hDC){
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                                              // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0,0,0,0,0,0,                            // Not used to select mode
		0,0,                                    // Not used to select mode
		0,0,0,0,0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0,0,0 };                                // Not used to select mode
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

HPALETTE GetOpenGLPalette(HDC hDC){
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange,GreenRange,BlueRange;
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;
	nColors = 1 << pfd.cColorBits;	
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size
	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;
	for(i = 0; i < nColors; i++)
		{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
		}
	hRetPal = CreatePalette(pPal);
	SelectPalette(hDC,hRetPal,FALSE);
	RealizePalette(hDC);
	free(pPal);
	return hRetPal;
}

int APIENTRY WinMain(   HINSTANCE       hInst,
						HINSTANCE       hPrevInstance,
						LPSTR           lpCmdLine,
						int                     nCmdShow){
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle
	hInstance = hInst;
	wc.style                        = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc          = (WNDPROC) WndProc;
	wc.cbClsExtra           = 0;
	wc.cbWndExtra           = 0;
	wc.hInstance            = hInstance;
	wc.hIcon                        = NULL;
	wc.hCursor                      = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground        = NULL;         
	wc.lpszMenuName         = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName        = lpszAppName;
	if(RegisterClass(&wc) == 0)
		return FALSE;
	hWnd = CreateWindow(
				lpszAppName,
				lpszAppName,
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
				50, 50,
				400, 400,
				NULL,
				NULL,
				hInstance,
				NULL);
	if(hWnd == NULL)
		return FALSE;
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
	while( GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
	return msg.wParam;
}
void move();
LRESULT CALLBACK WndProc(   HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam){
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context
	switch (message)
		{
		case WM_CREATE:
			hDC = GetDC(hWnd);              
			SetDCPixelFormat(hDC);          
			hPalette = GetOpenGLPalette(hDC);
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			SetupRC();
			SetTimer(hWnd, TIMER_ID, 16, NULL);
			SetTimer(hWnd, TIMER_COLLISION_COUNT_ID, COLLISION_COUNT_TIMER_INTERVAL, NULL);
			glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			
			bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);
			glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
						 bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
			if(bitmapData)
			free(bitmapData);
			bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
			glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
						 bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
			if(bitmapData)
			free(bitmapData);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
			break;
		case WM_DESTROY:
			wglMakeCurrent(hDC,NULL);
			wglDeleteContext(hRC);
			if(hPalette != NULL)
				DeleteObject(hPalette);
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			ChangeSize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_PAINT:
			{
			RenderScene();
			SwapBuffers(hDC);
			ValidateRect(hWnd,NULL);
			}
			break;
		case WM_QUERYNEWPALETTE:
			if(hPalette)
				{
				int nRet;
				SelectPalette(hDC, hPalette, FALSE);
				nRet = RealizePalette(hDC);
				InvalidateRect(hWnd,NULL,FALSE);
				return nRet;
				}
			break;
		case WM_PALETTECHANGED:
			if((hPalette != NULL) && ((HWND)wParam != hWnd))
				{
				SelectPalette(hDC,hPalette,FALSE);
				RealizePalette(hDC);
				UpdateColors(hDC);
				return 0;
				}
			break;
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
		}
		break;

		case WM_RBUTTONDOWN:
		{
			prevMousePos = MAKEPOINTS(lParam);
			dragging = true;
		}
		break;
		case WM_RBUTTONUP:
		{
			dragging = false;
		}
		break;
		case WM_TIMER:
			if (wParam == TIMER_ID) {
				move();
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (wParam == TIMER_COLLISION_COUNT_ID) {
				collisionCount += 10; // Increase collisionCount by 10 every 10 seconds
			}
			break;

		case WM_KEYDOWN:
			keysPressed.insert(wParam);
			if (wParam == 'W') isWKeyPressed = true;
			if (wParam == 'S') isSKeyPressed = true;
			move();
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_KEYUP:
			keysPressed.erase(wParam);
			if (wParam == 'W') isWKeyPressed = false;
			if (wParam == 'S') isSKeyPressed = false;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_MOUSEMOVE:
		{
			if (dragging) {
				POINTS currentMousePos = MAKEPOINTS(lParam);
				int deltaX = currentMousePos.x - prevMousePos.x;
				azimuth += angleJump * deltaX / 5;
				prevMousePos = currentMousePos;
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}
		break;
		case WM_COMMAND:
			{
			switch(LOWORD(wParam))
				{
				case ID_FILE_EXIT:
					DestroyWindow(hWnd);
					break;
				case ID_HELP_ABOUT:
					DialogBox (hInstance,
						MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
						hWnd,
						AboutDlgProc);
					break;
				}
			}
			break;
	default:   // Passes it on if unproccessed
	    return (DefWindowProc(hWnd, message, wParam, lParam));
	}
    return (0L);
}

INT_PTR APIENTRY AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		int i;
		GLenum glError;
		SetDlgItemText(hDlg, IDC_OPENGL_VENDOR, (LPCSTR)glGetString(GL_VENDOR));
		SetDlgItemText(hDlg, IDC_OPENGL_RENDERER, (LPCSTR)glGetString(GL_RENDERER));
		SetDlgItemText(hDlg, IDC_OPENGL_VERSION, (LPCSTR)glGetString(GL_VERSION));
		SetDlgItemText(hDlg, IDC_OPENGL_EXTENSIONS, (LPCSTR)glGetString(GL_EXTENSIONS));
		SetDlgItemText(hDlg, IDC_GLU_VERSION, (LPCSTR)gluGetString(GLU_VERSION));
		SetDlgItemText(hDlg, IDC_GLU_EXTENSIONS, (LPCSTR)gluGetString(GLU_EXTENSIONS));
		i = 0;
		do {
			glError = glGetError();
			SetDlgItemText(hDlg, IDC_ERROR1 + i, (LPCSTR)gluErrorString(glError));
			i++;
		} while (i < 6 && glError != GL_NO_ERROR);


		return (TRUE);
	}
	break;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK)
			EndDialog(hDlg, TRUE);
	}
	break;
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		break;
	}
	return FALSE;
}
static auto lastCollisionCheckTime = std::chrono::high_resolution_clock::now();
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
			azimuth += (speed/2 * (GL_PI / 180));
			rot -= speed/2;
		}
		else if (keysPressed.count('A') && !keysPressed.count('D')) {
			azimuth -= (speed/2 * (GL_PI / 180));
			rot += speed/2;
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
				azimuth += (speed/2 * 1.0 * (GL_PI / 180));
				rot -= speed/2 * 1.0f;
			}
			else if (keysPressed.count('A') && !keysPressed.count('D')) {
				azimuth -= (speed/2 * 1.0 * (GL_PI / 180));
				rot += speed/2 * 1.0f;
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