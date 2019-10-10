#include "NineMensMorris.h"
#include "ComputerPlayer.h"


int ComputerPlayer::heuristics(NineMensMorris game) {

	int sum = 0;
	for (auto elem : game.mens) {
		sum += elem.second;
	}
	return (int)game.mens[game.current_player] / sum;
}

std::map<NineMensMorris, int, cmpGames> moves;

NineMensMorris ComputerPlayer::MiniMax(NineMensMorris game, int player, int depth) {
	NineMensMorris best;
	int heur;
	if (game.state == NineMensMorris::State::Draw || game.state == NineMensMorris::State::Win || game.state == NineMensMorris::State::Lose
		|| depth > NineMensMorris::MAX_DEPTH) {
		heur = heuristics(game);
		return game;
	}

	
	else {
		int score = INT_MIN;
		switch (game.state)
		{
		case NineMensMorris::State::Placing:
			for (auto child : game.getPlaceBoards()) {
				int nextpl = child.nextPlayer();
				NineMensMorris n = MiniMax(child, child.current_player, depth + 1);
					int s = heuristics(n);
				if (s > score) {
					score = s;
					best = n;
				}
			}
			return best;
			break;
		case NineMensMorris::State::Moving:
			for (auto child : game.getMoveBoards()) {
				int nextpl = child.nextPlayer();
				NineMensMorris n = MiniMax(child, child.current_player, depth + 1);
				int s = heuristics(n);
				if (s > score) {
					score = s;
					best = n;
				}
			}

			return best;
			break;
		case NineMensMorris::State::Mill:
			for (auto child : game.getMillBoards()) {
				int nextpl = child.nextPlayer();
				NineMensMorris n = MiniMax(child, child.current_player, depth + 1);
				int s = heuristics(n);
				if (s > score) {
					score = s;
					best = n;
				}
			}
			return best;
			break;

		default:
			break;
		}
	}
}

NineMensMorris ComputerPlayer::getBestMove(NineMensMorris game) {
	NineMensMorris best = MiniMax(game, game.current_player, 0);
	return best;
}

void ComputerPlayer::make_actions(NineMensMorris& game)
{
	int move1, move2;
	switch (game.state)
	{
	case NineMensMorris::State::Placing:
	{
		//NineMensMorris minmax = MiniMax(game, game.current_player, 0);
		NineMensMorris best_move = getBestMove(game);
		game.place(best_move.prev_to);
		moves.clear();
	}
	break;
	case NineMensMorris::State::Mill:
	{
		//NineMensMorris minmax = MiniMax(game, game.current_player, 0);
		NineMensMorris best_move = getBestMove(game);
		game.mill(best_move.prev_to);
		moves.clear();
	}
	break;
	case NineMensMorris::State::Moving:
	{
		//NineMensMorris minmax = MiniMax(game, game.current_player, 0);
		NineMensMorris best_move = getBestMove(game);
		game.move(best_move.prev_from, best_move.prev_to);
		moves.clear();
	}
	break;
	default:
		break;
	}
}