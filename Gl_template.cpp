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

#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14

HPALETTE hPalette = NULL;

static LPCTSTR lpszAppName = "Kosiarka";
static HINSTANCE hInstance;

static GLfloat xCamPos = -0.5;
static GLfloat yCamPos = 0.025;
static GLfloat zCamPos = 0;
double angle = 0;
double vertAngle = 3.14159 / 4;
double const angleJump = 3.14159 / 16;
double const radiusJump = 0.01;
double radius = 0.1;

static GLsizei lastHeight;
static GLsizei lastWidth;

BITMAPINFOHEADER	bitmapInfoHeader;	// nag³ówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury

LRESULT CALLBACK WndProc(   HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam);
INT_PTR APIENTRY AboutDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void SetDCPixelFormat(HDC hDC);

void ChangeSize(GLsizei w, GLsizei h){
	GLfloat nRange = 400;
	GLfloat fAspect;
	if(h == 0)
		h = 1;
	lastWidth = w;
	lastHeight = h;
	fAspect=(GLfloat)w/(GLfloat)h;
    glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    if (w <= h) 
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else 
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetupRC(){
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
	glColor3f(0.0,0.0,0.0);
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

void RenderScene(void){
	glLoadIdentity(); //disables spinning
	//zoom
	xCamPos = xCamPos + ((radius * cos(vertAngle)) / sin(vertAngle));
	yCamPos = yCamPos + (radius/sin(vertAngle));
	//circle
	xCamPos = cos(angle) * radius;
	zCamPos = sin(angle) * radius;

	gluLookAt(xCamPos, yCamPos, zCamPos, 0, 0, 0, 0, 1, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	int centerX = 0;
	int centerY = 0;
	int centerZ = 0;

	glTranslatef(centerX, centerY, centerZ ); // Translate to position of Lazik
	glRotatef(0, 1.0f, 0.0f, 0.0f);
	glRotatef(0, 0.0f, 1.0f, 0.0f);
	glRotatef(0, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_BACK, GL_LINE);

	Lazik prost(50, 20, 10);
	prost.draw(0, 0, 0);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();

	glPushMatrix();
	glRotatef(0, 1.0f, 0.0f, 0.0f);
	glRotatef(0, 0.0f, 1.0f, 0.0f);
	glRotatef(0, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_BACK, GL_LINE);
	Terrain terrain;
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
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
		case WM_KEYDOWN:
			{
			//if (wParam == VK_UP)
			//	

			//if (wParam == VK_DOWN)
			//	//xRot += 5.0f;

			if (wParam == VK_LEFT) {
				angle -= angleJump;
				if (angle < 0) {
					angle = 3.14159 * 2;
				}
			}
			if (wParam == VK_RIGHT) {
				angle += angleJump;
				if (angle > 3.14159 * 2) {
					angle = 0;
				}
			}
			if (wParam == VK_UP) {
				radius -= radiusJump;
			}
			if (wParam == VK_DOWN) {
				radius += radiusJump;
			}
			InvalidateRect(hWnd,NULL,FALSE);
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

INT_PTR APIENTRY AboutDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
    switch (message)
	{
	    case WM_INITDIALOG:
			{
			int i;
			GLenum glError;
			SetDlgItemText(hDlg,IDC_OPENGL_VENDOR,(LPCSTR)glGetString(GL_VENDOR));
			SetDlgItemText(hDlg,IDC_OPENGL_RENDERER, (LPCSTR)glGetString(GL_RENDERER));
			SetDlgItemText(hDlg,IDC_OPENGL_VERSION, (LPCSTR)glGetString(GL_VERSION));
			SetDlgItemText(hDlg,IDC_OPENGL_EXTENSIONS, (LPCSTR)glGetString(GL_EXTENSIONS));
			SetDlgItemText(hDlg,IDC_GLU_VERSION, (LPCSTR)gluGetString(GLU_VERSION));
			SetDlgItemText(hDlg,IDC_GLU_EXTENSIONS, (LPCSTR)gluGetString(GLU_EXTENSIONS));
			i = 0;
			do {
				glError = glGetError();
				SetDlgItemText(hDlg,IDC_ERROR1+i, (LPCSTR)gluErrorString(glError));
				i++;
				}
			while(i < 6 && glError != GL_NO_ERROR);


			return (TRUE);
			}
			break;
	    case WM_COMMAND:      
			{
			if(LOWORD(wParam) == IDOK)
				EndDialog(hDlg,TRUE);
		    }
			break;
		case WM_CLOSE:
			EndDialog(hDlg,TRUE);
			break;
		}
	return FALSE;
}