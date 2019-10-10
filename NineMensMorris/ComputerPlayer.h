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
	int MiniMax(NineMensMorris game, int player, int depth) {
		if (game.state == NineMensMorris::State::Draw || game.state == NineMensMorris::State::Win || game.state == NineMensMorris::State::Lose
			|| depth > NineMensMorris::MAX_DEPTH) {
			return heuristics(game);
		}
		else {
			int score = INT_MIN;
			switch (game.state)
			{
			case NineMensMorris::State::Placing:
				for (auto child : game.getPlaceBoards()) {
					int s = MiniMax(child, game.nextPlayer(), depth + 1);
					if (s > score) {
						score = s;
						moves[child] = score;
					}
				}
				return score;
				break;
			case NineMensMorris::State::Moving:
				for (auto child : game.getMoveBoards()) {
					int s = MiniMax(child, game.nextPlayer(), depth + 1);
					if (s > score) {
						score = s;
						moves[child]= score;
					}
				}

				return score;
				break;
			case NineMensMorris::State::Mill:
				for (auto child : game.getMillBoards()) {
					int s = MiniMax(child, game.nextPlayer(), depth + 1);
					if (s > score) {
						score = s;
						moves[child] = score;
					}
				}
				return score;
				break;

			default:
				break;
			}
		}
	}

	int heuristics(NineMensMorris game) {
		return (int)game.mens[game.current_player] / game.placedCount;
	}

	NineMensMorris getBestMove() {
		int max = INT_MIN;
		NineMensMorris res;
		for (auto move : moves) {
			if (move.second > max){
				max = move.second;
				res = move.first;
			}
		}
		return res;
	}

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