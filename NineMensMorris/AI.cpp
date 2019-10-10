#include "NineMensMorris.h"
#include <iostream>
#include <vector>

int MiniMax(NineMensMorris game, int player, int depth) {
	if (game.state == NineMensMorris::State::Draw || game.state == NineMensMorris::State::Win || game.state == NineMensMorris::State::Lose
		|| depth > NineMensMorris::MAX_DEPTH) {
		return heuristics(game);
	}
	else {
		int score = MININT;
		switch (game.state)
		{
		case NineMensMorris::State::Placing:
			for (auto child : game.getPlaceBoards) {
				int s = MiniMax(child, game.nextPlayer, depth + 1);
				if (s > score) {
					score = s;
				}
			}
			return score;
			break;
		case NineMensMorris::State::Moving:
			for (auto child : game.getMoveBoards) {
				int s = MiniMax(child, game.nextPlayer, depth + 1);
				if (s > score) {
					score = s;
				}
			}

			return score;
			break;
		case NineMensMorris::State::Mill:
			for (auto child : game.getMillBoards) {
				int s = MiniMax(child, game.nextPlayer, depth + 1);
				if (s > score) {
					score = s;
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

void makemove(NineMensMorris game, int depth) {
	int score = MiniMax(game, game.current_player, 0);
}