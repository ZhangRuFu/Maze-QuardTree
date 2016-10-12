#pragma once
#include <list>

class View;

namespace DataStruct
{
	class Rectangle
	{
		int m_x, m_y;
		int m_width, m_height;

	public:
		Rectangle(int x = 0, int y = 0, int width = 0, int height = 0)
		{
			m_x = x;
			m_y = y;
			m_width = width;
			m_height = height;
		}

		int GetLeft(void) { return m_x; }
		int GetTop(void) { return m_y; }
		int GetRight(void) { return m_x + m_width; }
		int GetBottom(void) { return m_y + m_height; }
		int GetWidth(void) { return m_width; }
		int GetHeight(void) { return m_height; }
		void ChangeX(int delta) { m_x += delta; }
		void ChangeY(int delta) { m_y += delta; }
		void ChangeWidth(int delta) { m_width += delta; }
		void ChangeHeight(int delta) { m_height += delta; }
	};

	class QuardTree
	{
	private:
		int m_capacity;
		Rectangle m_area;

		std::list<Rectangle*> m_objects;
		QuardTree *m_subNodes;

	private:
		static const int MAX_CAPACITY = 5;
		static int s_depth;
	public:
		QuardTree();
		QuardTree(Rectangle area);

	public:
		void Init(Rectangle area);
		void Insert(Rectangle *rect);
		void Insert(std::list<Rectangle> &rects);
		void GetCollider(std::list<Rectangle*> *colliders, Rectangle rect);
		void Clear(void);

		static int GetDepth(void);

	private:
		const int* GetIndex(Rectangle rect);
		void Split(void);

		friend class View;
	};
}