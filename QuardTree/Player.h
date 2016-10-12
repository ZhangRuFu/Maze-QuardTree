#pragma once
#include "QuardTree.h"

using namespace DataStruct;
enum InputMessage { UP, DOWN, LEFT, RIGHT};

class Player
{
private:
	DataStruct::Rectangle *m_player;

public:
	Player(DataStruct::Rectangle rect);
	void Input(InputMessage message);
	DataStruct::Rectangle* GetRectangle(void);
};