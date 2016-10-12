#pragma once
#include <Windows.h>
#include <gdiplus.h>

class Vector2D;
class Maze;

class View
{
private:
	HWND m_hwnd;
	HDC m_hdc;
	Maze *m_maze;
	int m_screenWidth;
	int m_screenHeight;
	int m_cellWidth;
	int m_cellHeight;
	Gdiplus::Bitmap *m_windowBitmap;
	Gdiplus::Graphics *m_bitmapGraphics;
	Gdiplus::Graphics *m_windwoGraphics;

public:
	View(void);
	~View(void);
	void Init(HWND hwnd, Maze *maze);
	void WindowChang(int width, int height);
	void RenderFullMaze(void);
	void RenderFullFindedMaze(void);
	void RenderSingleFinedPoint(Vector2D *point, bool isDeleted);
};