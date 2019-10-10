#include "NineMensMorris.h"
#include "ComputerPlayer.h"


int heuristics(NineMensMorris game) {

	int sum = 0;
	for (auto elem : game.mens) {
		sum += elem.second;
	}
	return (int)game.mens[game.current_player] / sum;
}

std::map<NineMensMorris, int, cmpGames> moves;

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
					moves[child] = score;
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

NineMensMorris getBestMove() {
	int max = INT_MIN;
	NineMensMorris res;
	for (auto move : moves) {
		if (move.second > max) {
			max = move.second;
			res = move.first;
		}
	}
	return res;
}

