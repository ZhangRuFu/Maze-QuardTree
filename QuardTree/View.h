#pragma once
#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include "QuardTree.h"
#include "Player.h"

class View
{
private:
	HWND m_hwnd;
	HDC m_hdc;
	DataStruct::QuardTree *m_quardTree;

	int m_screenWidth;
	int m_screenHeight;

	Gdiplus::Bitmap *m_windowBitmap;
	Gdiplus::Graphics *m_bitmapGraphics;
	Gdiplus::Graphics *m_windwoGraphics;

public:
	Player *m_player;
	std::list<DataStruct::Rectangle*> *m_colliders ;

public:
	View(void);
	~View(void);
	void Init(HWND hwnd, DataStruct::QuardTree *quardTree);
	void WindowChang(int width, int height);
	void RenderQTFrame(void);
	void RenderAllRect(std::list<DataStruct::Rectangle> *rects);
	void RenderCollider(void);
	void Clear(void);
	void DrawFinish(void);

private:
	void DrawCross(DataStruct::Rectangle *rect);
};