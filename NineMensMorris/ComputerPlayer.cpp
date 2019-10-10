#include "NineMensMorris.h"
#include "ComputerPlayer.h"


double ComputerPlayer::heuristics(NineMensMorris game, int id) {

	int sum = 0;
	int mens = game.mens[id];
	for (auto elem : game.mens) {
		sum += elem.second;
	}
	return (mens * 1.0) / sum;
}


NineMensMorris ComputerPlayer::MiniMax(NineMensMorris game, int player, int depth) {
	NineMensMorris best;
	double heur;
	if (game.state == NineMensMorris::State::Draw || game.state == NineMensMorris::State::Win || game.state == NineMensMorris::State::Lose
		|| depth > NineMensMorris::MAX_DEPTH) {
		heur = heuristics(game, player);
		return game;
	}


	else {
		double score = game.current_player == player ? DBL_MIN : DBL_MAX;
		switch (game.state)
		{
		case NineMensMorris::State::Placing:
			for (auto child : game.getPlaceBoards()) {
				int nextpl = child.nextPlayer();
				NineMensMorris n = MiniMax(child, player, depth + 1);
				double s = heuristics(n, player);
				bool comparison;
				comparison =  game.current_player == player ?  s > score : s < score;
				if (comparison) {
					score = s;
					best = child;
				}
			}
			if (game.getPlaceBoards().size() == 0) {
				return game;
			}
			return best;
			break;
		case NineMensMorris::State::Moving:
			for (auto child : game.getMoveBoards()) {
				int nextpl = child.nextPlayer();
				NineMensMorris n = MiniMax(child, player, depth + 1);
				int s = heuristics(n, player);
				bool comparison;
				comparison = game.current_player == player ? s > score : s < score;
				if (comparison) {
					score = s;
					best = child;
				}
			}

			return best;
			break;
		case NineMensMorris::State::Mill:
			for (auto child : game.getMillBoards()) {
				int nextpl = child.nextPlayer();
				NineMensMorris n = MiniMax(child, player, depth + 1);
				int s = heuristics(n, player);
				bool comparison;
				comparison = game.current_player == player ? s > score : s < score;
				if (comparison) {
					score = s;
					best = child;
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
	}
	break;
	case NineMensMorris::State::Mill:
	{
		//NineMensMorris minmax = MiniMax(game, game.current_player, 0);
		NineMensMorris best_move = getBestMove(game);
		game.mill(best_move.prev_to);
	}
	break;
	case NineMensMorris::State::Moving:
	{
		NineMensMorris best_move = getBestMove(game);
		game.move(best_move.prev_from, best_move.prev_to);
	}
	break;
	default:
		break;
	}
}