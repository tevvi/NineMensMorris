#include "NineMensMorris.h"
#include "ComputerPlayer.h"


int ComputerPlayer::heuristics(NineMensMorris game) {

	int sum = 0;
	for (auto elem : game.mens) {
		sum += elem.second;
	}
	return (int)game.mens[game.current_player] / sum;
}

std::map<NineMensMorris, int> moves;

int ComputerPlayer::MiniMax(NineMensMorris game, int player, int depth) {
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

NineMensMorris ComputerPlayer::getBestMove() {
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

void ComputerPlayer::make_actions(NineMensMorris& game)
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