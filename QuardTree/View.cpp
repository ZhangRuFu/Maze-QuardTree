#include "View.h"
#include "QuardTree.h"
#include <stack>
#include <Windows.h>
#include <gdiplus.h>

using namespace Gdiplus;
using std::stack;

View::View(void)
{
	m_hwnd = nullptr;
	m_hdc = nullptr;
	m_quardTree = nullptr;
	m_player = new Player(DataStruct::Rectangle(260, 260, 20, 20));
	m_colliders = new std::list<DataStruct::Rectangle*>;
}

View::~View(void)
{
	//析构资源
}

void View::Init(HWND hwnd, DataStruct::QuardTree *quardTree)
{
	m_hwnd = hwnd;
	m_hdc = GetDC(hwnd);
	m_quardTree = quardTree;

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
}

void View::RenderQTFrame(void)
{
	//非先序遍历四叉树
	stack<DataStruct::QuardTree*> nodeStack;
	stack<int> indexStack;
	DataStruct::QuardTree *currentNode = m_quardTree;
	int index = 0;
	int sum = 0;
	if (m_quardTree->m_subNodes == nullptr)
		return;
	do
	{
		//结点第一次被遍历
		if (index == 0 && currentNode->m_subNodes != nullptr)
			DrawCross(&currentNode->m_area);
		//回滚
		if (currentNode->m_subNodes == nullptr || index == 4)
		{
			currentNode = nodeStack.top();
			index = indexStack.top();
			nodeStack.pop();
			indexStack.pop();
		}
		else
		{
			nodeStack.push(currentNode);
			indexStack.push(index + 1);
			currentNode = &currentNode->m_subNodes[index];
			index = 0;
		}
	} while (!nodeStack.empty() || index != 4);
}

void View::RenderAllRect(std::list<DataStruct::Rectangle> *rects)
{
	SolidBrush brush(Color(0, 0, 200));
	for (std::list<DataStruct::Rectangle>::iterator i = rects->begin(); i != rects->end(); i++)
		m_bitmapGraphics->FillRectangle(&brush, (*i).GetLeft(), (*i).GetTop(), (*i).GetWidth(), (*i).GetHeight());
	brush.SetColor(Color(0, 255, 0));
	m_bitmapGraphics->FillRectangle(&brush, m_player->GetRectangle()->GetLeft(), m_player->GetRectangle()->GetTop(), m_player->GetRectangle()->GetWidth(), m_player->GetRectangle()->GetHeight());
}

void View::RenderCollider(void)
{
	SolidBrush brush(Color(255, 0, 0));
	for (std::list<DataStruct::Rectangle*>::iterator i = m_colliders->begin(); i != m_colliders->end(); i++)
		m_bitmapGraphics->FillRectangle(&brush, (*i)->GetLeft(), (*i)->GetTop(), (*i)->GetWidth(), (*i)->GetHeight());
}

void View::Clear(void)
{
	m_bitmapGraphics->Clear(Color(0, 0, 0));
}

void View::DrawFinish(void)
{
	m_windwoGraphics->DrawImage(m_windowBitmap,0 , 0, m_screenWidth, m_screenHeight);
}

void View::DrawCross(DataStruct::Rectangle * rect)
{
	Pen pen(Color(255, 255, 255));
	int startX = rect->GetLeft();
	int startY = rect->GetTop() + rect->GetHeight() / 2;
	int endX = rect->GetRight();
	int endY = startY;
	m_bitmapGraphics->DrawLine(&pen, startX, startY, endX, endY);
	startX = endX = rect->GetLeft() + rect->GetWidth() / 2;
	startY = rect->GetTop();
	endY = rect->GetBottom();
	m_bitmapGraphics->DrawLine(&pen, startX, startY, endX, endY);
}

