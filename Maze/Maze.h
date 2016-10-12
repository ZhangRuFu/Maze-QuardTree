#pragma once
#include <stack>
#include <list>
#include <windows.h>

enum Direction { UP, DOWN, LEFT, RIGHT, COUNT };

class Vector2D
{
public:
	int m_x, m_y;
	Vector2D(int x = 0, int y = 0)
	{
		m_x = x;
		m_y = y;
	}
};

class MazeRoom
{
	bool m_up : 1;
	bool m_down : 1;
	bool m_left : 1;
	bool m_right : 1;
	bool m_flag : 1;
	bool m_findFlag : 1;

public:
	MazeRoom(void)
	{
		Init();
	}

	void OpenDoor(Direction direction)
	{
		switch (direction)
		{
		case Direction::UP:
			m_up = true;
			break;
		case Direction::DOWN:
			m_down = true;
			break;
		case Direction::LEFT:
			m_left = true;
			break;
		case Direction::RIGHT:
			m_right = true;
			break;
		}
		m_flag = true;
	}

	bool IsDoorOpen(Direction direction)
	{
		switch (direction)
		{
		case Direction::UP:
			return m_up;
			break;
		case Direction::DOWN:
			return m_down;
			break;
		case Direction::LEFT:
			return m_left;
			break;
		case Direction::RIGHT:
			return m_right;
			break;
		}
		return false;
	}

	bool IsEmpty(void)
	{
		return !m_flag;
	}

	bool IsFinded(void)
	{
		return m_findFlag;
	}

	void Finded(void)
	{
		m_findFlag = true;
	}

	void Init(void)
	{
		m_up = false;
		m_down = false;
		m_left = false;
		m_right = false;
		m_flag = false;
		m_findFlag = false;
	}
};

class Maze
{
private:
	MazeRoom **m_maze;
	int m_currentX, m_currentY;
	int m_nextX, m_nextY;
	int m_width, m_height;
	int m_restCount;
	std::stack<Vector2D*> m_stack;
	std::list<Vector2D*> m_pathRecord;

public:
	void CreateMaze_DeepFirst(void);
	void FindMazePath(void);
	Maze(int width, int height);
	void InitMaze(void);

private:
	bool SeekNextRoom(void);
	void DestroyWall(void);
	void MoveToNextRoom(void);
	void RollBackRoom(void);
	void ClearStack(void);
	int GetValidPath(void);

	friend class View;
};

