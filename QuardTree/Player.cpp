#include "Player.h"

Player::Player(Rectangle rect)
{
	m_player = new Rectangle(rect);
}

void Player::Input(InputMessage message)
{
	switch (message)
	{
	case InputMessage::UP:
		m_player->ChangeY(-3);
		break;
	case InputMessage::DOWN:
		m_player->ChangeY(3);
		break;
	case InputMessage::LEFT:
		m_player->ChangeX(-3);
		break;
	case InputMessage::RIGHT:
		m_player->ChangeX(3);
		break;
	}
}

Rectangle * Player::GetRectangle(void)
{
	return m_player;
}
