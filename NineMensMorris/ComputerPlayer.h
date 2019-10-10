#pragma once
#include <iostream>
#include "Player.h"
#include "NineMensMorris.h"
#include <vector>
#include <tuple>
#include <map>


class ComputerPlayer : public Player<NineMensMorris>
{
public:
	std::map<NineMensMorris, int> moves;
	int MiniMax(NineMensMorris game, int player, int depth);

	int heuristics(NineMensMorris game);

	NineMensMorris getBestMove();

	virtual void make_actions(NineMensMorris& game) override
	{
		int move1, move2;
		switch (game.state)
		{
		case NineMensMorris::State::Placing:
		{
			int minmax = MiniMax(game, game.current_player, 5);
			NineMensMorris best_move = getBestMove();
			game.place(best_move.prev_to);
		}
			break;
		case NineMensMorris::State::Mill:
		{
			int minmax = MiniMax(game, game.current_player, 5);
			NineMensMorris best_move = getBestMove();
			game.mill(best_move.prev_to);
		}
			break;
		case NineMensMorris::State::Moving:
		{
			int minmax = MiniMax(game, game.current_player, 5);
			NineMensMorris best_move = getBestMove();
			game.move(best_move.prev_from, best_move.prev_to);
		}
			break;
		default:
			break;
		}
	}


};