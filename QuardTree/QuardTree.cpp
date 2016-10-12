#include "QuardTree.h"

using DataStruct::QuardTree;
using DataStruct::Rectangle;

QuardTree::QuardTree(Rectangle area) : m_area(area.GetLeft(), area.GetTop(), area.GetWidth(), area.GetHeight())
{
	m_capacity = 0;
	m_subNodes = nullptr;
}

QuardTree::QuardTree()
{
	m_capacity = 0;
	m_subNodes = nullptr;
}

void QuardTree::Init(Rectangle area)
{
	m_area = area;
}


void QuardTree::Insert(Rectangle * rect)
{
	//自己已经进行了分裂
	if (m_subNodes != nullptr)
	{
		const int* index = GetIndex(*rect);
		int i = 0;
		while (index[i] != -1)
			m_subNodes[index[i++]].Insert(rect);
		return;
	}

	//未分割
	m_objects.push_back(rect);
	++m_capacity;
	//超过最大容量，进行分裂
	if (m_subNodes == nullptr && m_capacity > QuardTree::MAX_CAPACITY)
	{
		Split();
		m_capacity = 0;
		for (std::list<Rectangle*>::iterator i = m_objects.begin(); i != m_objects.end(); )
		{
			const int *index = GetIndex(*(*i));
			int j = 0;
			while (index[j] != -1)
				m_subNodes[index[j++]].Insert(*i);
			i = m_objects.erase(i);
		}
	}
}

void DataStruct::QuardTree::Insert(std::list<Rectangle>& rects)
{
	for (std::list<Rectangle>::iterator i = rects.begin(); i != rects.end(); i++)
		Insert(&(*i));
}

void QuardTree::GetCollider(std::list<Rectangle*>* colliders, Rectangle rect)
{
	if (m_subNodes == nullptr)
	{
		for (std::list<Rectangle*>::iterator i = m_objects.begin(); i != m_objects.end();i++)
			colliders->push_back(*i);
		return;
	}
	const int *index = GetIndex(rect);
	int i = 0;
	while (index[i] != -1)
		m_subNodes[index[i++]].GetCollider(colliders, rect);
	return;
}

void QuardTree::Clear(void)
{
	m_objects.clear();
	if (m_subNodes == nullptr)
		return;
	for (int i = 0; i < 4; i++)
		m_subNodes[i].Clear();
	delete[] m_subNodes;
}

int DataStruct::QuardTree::GetDepth(void)
{
	return QuardTree::s_depth;
}


const int* QuardTree::GetIndex(Rectangle rect)
{
	const static int lu[2] = { 3, -1 };
	const static int mu[3] = { 0, 3, -1 };
	const static int ru[2] = { 0, -1 };

	const static int lm[3] = { 2, 3, -1 };
	const static int mm[5] = { 0, 1, 2, 3, -1 };
	const static int rm[3] = { 0, 1, -1 };

	const static int lb[2] = { 2, -1 };
	const static int mb[3] = { 1, 2, -1};
	const static int rb[2] = { 1, -1 };

	static const int *index[3][3] = { {lu, mu, ru}, {lm, mm, rm}, {lb, mb, rb} };

	int midPointX = m_area.GetLeft() + m_area.GetWidth() / 2;
	int midPointY = m_area.GetTop() + m_area.GetHeight() / 2;
	int bottom = 0, right = 0;
	if (rect.GetLeft() >= midPointX)
		right = 2;
	else if (rect.GetRight() <= midPointX)
		right = 0;
	else
		right = 1;

	if (rect.GetTop() >= midPointY)
		bottom = 2;
	else if (rect.GetBottom() <= midPointY)
		bottom = 0;
	else
		bottom = 1;
	
	return index[bottom][right];
}

void QuardTree::Split(void)
{
	int width = m_area.GetWidth() / 2;
	int height = m_area.GetHeight() / 2;
	int x = m_area.GetLeft();
	int y = m_area.GetTop();

	m_subNodes = new QuardTree[4]
	{
		QuardTree(Rectangle(x + width, y, width, height)),
		QuardTree(Rectangle(x + width, y + height, width, height)),
		QuardTree(Rectangle(x, y + height, width, height)),
		QuardTree(Rectangle(x, y, width, height))
	};
	QuardTree::s_depth++;
}

int QuardTree::s_depth = 0;