#pragma once
#include <iostream>
#include "Player.h"
#include "NineMensMorris.h"


class HumanPlayer : public Player<NineMensMorris>
{
	std::vector<NineMensMorris::ActionType> move_dec =
	{ {0, 0}, {0, 3}, {0, 6},
	  {1, 1}, {1, 3}, {1, 5},
	  {2, 2}, {2, 3}, {2, 4},
	  {3, 0}, {3, 1}, {3, 2},
	  {3, 4}, {3, 5}, {3, 6},
	  {4, 2}, {4, 3}, {4, 4},
	  {5, 1}, {5, 3}, {5, 5},
	  {6, 0}, {6, 3}, {6, 6} };

	virtual void make_actions(NineMensMorris& game) override
	{
		int move1, move2;
		switch (game.state)
		{
		case NineMensMorris::State::Placing:
			std::cin >> move1;
			game.place(move_dec[move1]);
			break;
		case NineMensMorris::State::Mill:
			std::cin >> move1;
			game.mill(move_dec[move1]);
			break;
		case NineMensMorris::State::Moving:
			std::cin >> move1;
			std::cin >> move2;
			game.move(move_dec[move1], move_dec[move2]);
			break;
		default:
			break;
		}
	}
};