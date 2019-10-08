#pragma once
#include <iostream>
#include "Player.h"
#include "NineMensMorris.h"


class HumanPlayer : public Player<NineMensMorris>
{
	virtual void make_actions(NineMensMorris& game) override
	{
		switch (game.state)
		{
		case NineMensMorris::State::Placing:
		default:
			throw base::NotImplementedException();
		}
	}
};