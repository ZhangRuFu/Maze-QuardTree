#include "Maze.h"
#include "View.h"
#include <vector>
#include <stdlib.h>

extern View view;
using std::vector;

Maze::Maze(int width, int height)
{
	m_height = height;
	m_width = width;
	
	InitMaze();
}

void Maze::InitMaze(void)
{
	if (m_maze == nullptr)
	{
		m_maze = new MazeRoom*[m_height];
		for (int i = 0; i < m_height; i++)
			m_maze[i] = new MazeRoom[m_width];
	}
	else
	{
		for (int i = 0; i < m_height; i++)
			for (int j = 0; j < m_width; j++)
				m_maze[i][j].Init();
	}
	m_pathRecord.clear();
	m_currentX = 0;
	m_currentY = 0;
	m_restCount = m_width * m_height - 1;
}

//搜寻当前位置的下一个房间
bool Maze::SeekNextRoom(void)
{
	vector<Vector2D> room;

	//填充房间数组
	int roomIndex = 0;
	if (m_currentY != 0)
	{
		room.push_back(Vector2D(m_currentX, m_currentY - 1));
	}
	if (m_currentY != m_height - 1)
	{
		room.push_back(Vector2D(m_currentX, m_currentY + 1));
	}
	if (m_currentX != 0)
	{
		room.push_back(Vector2D(m_currentX - 1, m_currentY));
	}
	if (m_currentX != m_width - 1)
	{
		room.push_back(Vector2D(m_currentX + 1, m_currentY));
	}

	int i = rand() % room.size();
	for (int count = 0; count < room.size(); count++, i++, i %= room.size())
		if (m_maze[room[i].m_y][room[i].m_x].IsEmpty())
		{
			m_nextX = room[i].m_x;
			m_nextY = room[i].m_y;
			return true;
		}
	return false;
}

//摧毁当前位置和下一位置的墙
void Maze::DestroyWall(void)
{
	if (m_nextX > m_currentX)
	{
		m_maze[m_currentY][m_currentX].OpenDoor(Direction::RIGHT);
		m_maze[m_nextY][m_nextX].OpenDoor(Direction::LEFT);
	}
	else if(m_nextX < m_currentX)
	{
		m_maze[m_currentY][m_currentX].OpenDoor(Direction::LEFT);
		m_maze[m_nextY][m_nextX].OpenDoor(Direction::RIGHT);
	}
	else if (m_nextY > m_currentY)
	{
		m_maze[m_currentY][m_currentX].OpenDoor(Direction::DOWN);
		m_maze[m_nextY][m_nextX].OpenDoor(Direction::UP);
	}
	else if(m_nextY < m_currentY)
	{
		m_maze[m_currentY][m_currentX].OpenDoor(Direction::UP);
		m_maze[m_nextY][m_nextX].OpenDoor(Direction::DOWN);
	}
}

//移动到下一房间
void Maze::MoveToNextRoom(void)
{
	Vector2D *newPosition = new Vector2D(m_currentX, m_currentY);
	m_stack.push(newPosition);
	m_currentX = m_nextX;
	m_currentY = m_nextY;
	--m_restCount;
}

//回滚到上一个房间
void Maze::RollBackRoom(void)
{
	Vector2D *position = m_stack.top();
	m_stack.pop();
	m_currentX = position->m_x;
	m_currentY = position->m_y;
	delete position;
}

void Maze::ClearStack(void)
{
	while (m_stack.size())
	{
		Vector2D *position = m_stack.top();
		m_stack.pop();
		delete position;
	}
}

//深度优先算法创建迷宫
void Maze::CreateMaze_DeepFirst(void)
{
	while (m_restCount)
	{
		if (SeekNextRoom())
		{
			DestroyWall();
			MoveToNextRoom();
		}
		else
			RollBackRoom();
	}
}

//迷宫求解
void Maze::FindMazePath(void)
{
	ClearStack();
	int count = 0;
	m_currentX = m_nextX = m_currentY = m_nextY = 0;
	m_pathRecord.push_back(new Vector2D(m_currentX, m_currentY));
	view.RenderSingleFinedPoint(new Vector2D(m_currentX, m_currentY), false);
	while (m_currentX != m_width - 1 || m_currentY != m_height - 1)
	{
		count++;
		int choose = GetValidPath();
		if (!choose)
		{
			//回滚
			view.RenderSingleFinedPoint(new Vector2D(m_currentX, m_currentY), true);
			Vector2D *position = m_pathRecord.back();
			m_pathRecord.pop_back();
			m_currentX = position->m_x;
			m_currentY = position->m_y;
			
			delete position;
		}
		else
		{
			m_pathRecord.push_back(new Vector2D(m_currentX, m_currentY));
			m_currentX = m_nextX;
			m_currentY = m_nextY;
			view.RenderSingleFinedPoint(new Vector2D(m_currentX, m_currentY), false);
		}
		Sleep(40);
	}
	m_pathRecord.push_back(new Vector2D(m_currentX, m_currentY));
}

//寻找下一个可到达的房间
int Maze::GetValidPath(void)
{
	static Vector2D *prePosition = new Vector2D[Direction::COUNT]
	{
		Vector2D(0, -1),
		Vector2D(0, 1),
		Vector2D(-1, 0),
		Vector2D(1, 0)
	};

	int nextX = 0;
	int nextY = 0;
	int choose = 0;

	int i = rand() % Direction::COUNT;
	for (int count = 0; count < Direction::COUNT; count++, i++, i %= Direction::COUNT)
		if (m_maze[m_currentY][m_currentX].IsDoorOpen((Direction)i))
		{
			nextX = m_currentX + prePosition[i].m_x;
			nextY = m_currentY + prePosition[i].m_y;
			if (!m_maze[nextY][nextX].IsFinded())
			{
				if (!choose)
				{
					m_nextX = nextX;
					m_nextY = nextY;
					m_maze[m_nextY][m_nextX].Finded();
				}
				choose++;
			}
		}
	return choose;
}