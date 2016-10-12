#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "Maze.h"
#include "View.h"

#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;
using std::wstring;

Maze maze(20, 20);
View view;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdStr, int nCmdShow)
{
	WNDCLASS wndClass;
	wstring wndClassName = L"WindowClass";
	wstring windowName = L"迷宫生成&求解";

	//GDI+初始化
	GdiplusStartupInput input;
	ULONG_PTR token;
	GdiplusStartup(&token, &input, nullptr);
	srand(time(nullptr));

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpszClassName = wndClassName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, L"IDC_ARROW");
	wndClass.hIcon = LoadIcon(NULL, L"IDI_APPLICATION");

	if (!RegisterClass(&wndClass))
	{
		MessageBox(NULL, L"Register wndClass Failed!", L"Error", MB_OK);
		return 1;
	}

	RECT windowRect;
	SetRect(&windowRect, 0, 0, 500, 500);
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	HWND hWnd = CreateWindow(wndClassName.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW & ~ WS_THICKFRAME, 50, 50, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	GdiplusShutdown(token);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_CREATE:
		view.Init(hwnd, &maze);
		maze.CreateMaze_DeepFirst();
		break;
	case WM_LBUTTONDOWN:
		maze.FindMazePath();
		MessageBox(hwnd, L"Get it!", L"迷宫", MB_OK);
		InvalidateRect(hwnd, nullptr, false);
		break;
	case WM_RBUTTONDOWN:
		maze.InitMaze();
		maze.CreateMaze_DeepFirst();
		InvalidateRect(hwnd, nullptr, false);
		UpdateWindow(hwnd);
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		view.RenderFullMaze();
		view.RenderFullFindedMaze();
		EndPaint(hwnd, &ps);
		break;
	case WM_SIZE:
		view.WindowChang(LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
		break;
	}
	return 0;
}
