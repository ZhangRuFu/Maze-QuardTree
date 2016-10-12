#include "View.h"
#include "Maze.h"
#include <Windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

using Gdiplus::Point;

View::View(void)
{
	m_hwnd = nullptr;
	m_hdc = nullptr;
	m_maze = nullptr;

	m_cellHeight = m_cellWidth = m_screenHeight = m_screenWidth;
}

View::~View(void)
{
	//Îö¹¹×ÊÔ´
}

void View::Init(HWND hwnd, Maze *maze)
{
	m_hwnd = hwnd;
	m_hdc = GetDC(hwnd);
	m_maze = maze;

	RECT rect;
	GetClientRect(hwnd, &rect);
	WindowChang(rect.right - rect.left, rect.bottom - rect.top);

	m_windowBitmap = new Bitmap(m_screenWidth, m_screenHeight);
	m_bitmapGraphics = Graphics::FromImage(m_windowBitmap);
	m_windwoGraphics = new Graphics(m_hdc);
}

void View::WindowChang(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
	m_cellWidth = m_screenWidth / m_maze->m_width;
	m_cellHeight = m_screenHeight / m_maze->m_height;
}

void View::RenderFullMaze(void)
{
	SolidBrush brush(Color(255, 255, 255));
	m_bitmapGraphics->Clear(Color(0, 0, 0));

	for (int i = 0; i < m_maze->m_height; i++)
		for (int j = 0; j < m_maze->m_width; j++)
		{
			int startX = 1;
			int startY = 1;
			int endX = m_cellWidth - 1;
			int endY = m_cellHeight - 1;
			MazeRoom *room = &(m_maze->m_maze[i][j]);
			if (room->IsDoorOpen(Direction::UP))
				startY -= 1;
			if (room->IsDoorOpen(Direction::DOWN))
				endY += 1;
			if (room->IsDoorOpen(Direction::LEFT))
				startX -= 1;
			if (room->IsDoorOpen(Direction::RIGHT))
				endX += 1;
			m_bitmapGraphics->FillRectangle(&brush, j * m_cellWidth + startX, i * m_cellHeight + startY, endX - startX, endY - startY);
		}
	m_windwoGraphics->DrawImage(m_windowBitmap, 0, 0);
}

void View::RenderFullFindedMaze(void)
{
	Pen pen(Color(255, 0, 0));
	Point *point = new Point[m_maze->m_pathRecord.size()];
	int index = 0;
	for (std::list<Vector2D*>::iterator i = m_maze->m_pathRecord.begin(); i != m_maze->m_pathRecord.end(); i++, index++)
	{
		point[index].X = m_cellWidth / 2 + (*i)->m_x * m_cellWidth;
		point[index].Y = m_cellHeight / 2 + (*i)->m_y * m_cellHeight;
	}
	m_bitmapGraphics->DrawLines(&pen, point, m_maze->m_pathRecord.size());


	m_windwoGraphics->DrawImage(m_windowBitmap, 0, 0);
}

void View::RenderSingleFinedPoint(Vector2D *point, bool isDeleted)
{
	SolidBrush brushEllipse(Color(0x99, 0xff, 0xff));
	SolidBrush brushRect(Color(255, 255, 255));
	Point *circle = new Point();
	circle->X = m_cellWidth / 4 + point->m_x * m_cellWidth;
	circle->Y = m_cellHeight / 4 + point->m_y * m_cellHeight;
	if(isDeleted)
		m_bitmapGraphics->FillRectangle(&brushRect, circle->X, circle->Y, m_cellWidth / 2, m_cellHeight / 2);
	else
		m_bitmapGraphics->FillEllipse(&brushEllipse, circle->X, circle->Y, m_cellWidth / 2, m_cellHeight / 2);
	
	m_windwoGraphics->DrawImage(m_windowBitmap, 0, 0, m_screenWidth, m_screenHeight);
	delete circle;
}
