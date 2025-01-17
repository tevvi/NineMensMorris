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


NineMensMorris ComputerPlayer::MiniMax(NineMensMorris game, int player, int depth, double alpha, double beta) {
	NineMensMorris best;
	double heur;
	if (game.state == NineMensMorris::State::Draw || game.state == NineMensMorris::State::Win || game.state == NineMensMorris::State::Lose
		|| depth > game.MAX_DEPTH) {
		game.heur = heuristics(game, player);
		return game;
	}


	else {
		double s;
		double score = game.current_player == player ? DBL_MIN : DBL_MAX;
		int nextPlayer = game.current_player == 1 ? 2 : 1;
		bool comparison;
		switch (game.state)
		{
		case NineMensMorris::State::Placing:
			for (auto child : game.getPlaceBoards()) {
				child.nextPlayer();
				NineMensMorris n;
				n = MiniMax(child, player, depth + 1, alpha, beta);
				s = n.heur;
				comparison =  game.current_player == player ?  s > score : s < score;
				if (comparison) {
					score = s;
					child.heur = s;
					best = child;
				}
				if (game.current_player == player) {
					alpha = alpha > s ? alpha : s;
				}
				else {
					beta = beta < s ? beta : s;
				}
				if (beta < alpha) {
					break;
				}
			}
			return best;
			break;
		case NineMensMorris::State::Moving:
			for (auto child : game.getMoveBoards()) {
				child.nextPlayer();
				NineMensMorris n = MiniMax(child, player, depth + 1, alpha, beta);
				s = n.heur;
				comparison = game.current_player == player ? s > score : s < score;
				if (comparison) {
					score = s;
					child.heur = s;
					best = child;
				}
				if (game.current_player == player) {
					alpha = alpha > s ? alpha : s;
				}
				else {
					beta = beta < s ? beta : s;
				}
				if (beta < alpha) {
					break;
				}
			}

			return best;
			break;
		case NineMensMorris::State::Mill:
			for (auto child : game.getMillBoards()) {
				child.nextPlayer();
				NineMensMorris n = MiniMax(child, player, depth + 1, alpha, beta);
				s = n.heur;
				comparison = game.current_player == player ? s > score : s < score;
				if (comparison) {
					score = s;
					child.heur = s;
					best = child;
				}
				if (game.current_player == player) {
					alpha = alpha > s ? alpha : s;
				}
				else {
					beta = beta < s ? beta : s;
				}
				if (beta < alpha) {
					break;
				}
			}
			return best;
			break;
		case NineMensMorris::State::Halt:
		{
			auto child = game;
			child.nextPlayer();
			best = MiniMax(child, player, depth + 1, alpha, beta);
			game.heur = best.heur;
			return best;
			break;
		}
		default:
			break;
		}
	}
}

NineMensMorris ComputerPlayer::getBestMove(NineMensMorris game) {
	NineMensMorris best = MiniMax(game, game.current_player, 0, DBL_MIN, DBL_MAX);
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
		if (game.placedCount == 16 || game.placedCount == 17) {
			game.MAX_DEPTH = 6;
		}
		NineMensMorris best_move = getBestMove(game);
		game.place(best_move.prev_to);
		break;
	}
	case NineMensMorris::State::Mill:
	{
		//NineMensMorris minmax = MiniMax(game, game.current_player, 0);
		NineMensMorris best_move = getBestMove(game);
		game.mill(best_move.prev_to);
		break;
	}
	case NineMensMorris::State::Moving:
	{
		NineMensMorris best_move = getBestMove(game);
		game.move(best_move.prev_from, best_move.prev_to);
		break;
	}
	case NineMensMorris::State::Halt:
	{
		game.halt();
		break;
	}
	default:
		break;
	}
}