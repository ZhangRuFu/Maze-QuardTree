#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include <list>
#include <stdlib.h>
#include <time.h>
#include "QuardTree.h"
#include "View.h"
#include "Player.h"

#define MAX_COUNT 100
#define MAX_WIDTH 10
#define MAX_HEIGHT 10

#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;
using std::wstring;
using std::list;
using DataStruct::QuardTree;

//全局变量
QuardTree quardTree;
View view;
list<DataStruct::Rectangle> rectangles;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void CreateRectangle(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdStr, int nCmdShow)
{
	WNDCLASS wndClass;
	wstring wndClassName = L"WindowClass";
	wstring windowName = L"四叉树空间分割-碰撞检测";

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

	HWND hWnd = CreateWindow(wndClassName.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW, 50, 50, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

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
	static bool cross = false;
	static const InputMessage input[4] = { InputMessage::LEFT, InputMessage::UP, InputMessage::RIGHT, InputMessage::DOWN,   };
	switch (message)
	{
	case WM_CREATE:
		view.Init(hwnd, &quardTree);
		quardTree.Init(DataStruct::Rectangle(0, 0, 500, 500));
		CreateRectangle();
		quardTree.Insert(rectangles);
		quardTree.GetCollider(view.m_colliders, *view.m_player->GetRectangle());
		MessageBox(hwnd, L"初始化--OK!\n插入矩形--OK!\n获取碰撞体--OK", L"四叉树空间分割", MB_OK);
		break;
	case WM_KEYDOWN:
		view.m_player->Input(input[wparam - VK_LEFT]);
		view.m_colliders->clear();
		quardTree.GetCollider(view.m_colliders, *view.m_player->GetRectangle());
		InvalidateRect(hwnd, nullptr, false);
		UpdateWindow(hwnd);
		break;
	case WM_LBUTTONDOWN:
		cross = true;
		InvalidateRect(hwnd, nullptr, false);
		UpdateWindow(hwnd);
		MessageBox(hwnd, L"渲染四叉树框架--OK!", L"四叉树空间分割", MB_OK);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		view.Clear();
		view.RenderAllRect(&rectangles);
		if(cross) 
			view.RenderQTFrame();
		view.RenderCollider();
		view.DrawFinish();
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

void CreateRectangle(void)
{
	int x, y;
	int width, height;
	for (int i = 0; i < MAX_COUNT; i++)
	{
		x = rand() % 480;
		y = rand() % 480;
		width = 10;//5 + rand() % (MAX_WIDTH - 5);
		height = 10;//5 + rand() % (MAX_HEIGHT - 5);
		rectangles.push_back(DataStruct::Rectangle(x, y, width, height));
	}
	
}