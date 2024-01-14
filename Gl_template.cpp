#include "loader.h"
#include "resource.h"
#include <chrono>
#include <Game.h>

Game game;
static HINSTANCE hInstance;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return game.eventListener(hWnd, message, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	game.setStartTime(std::chrono::high_resolution_clock::now());
	MSG msg;
	WNDCLASS wc = { 0 };
	hInstance = hInst;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = game.lpszAppName;
	if (RegisterClass(&wc) == 0) return FALSE;
	HWND hWnd = CreateWindow(game.lpszAppName, game.lpszAppName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 50, 50, game.screenX, game.screenY, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) return FALSE;
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}