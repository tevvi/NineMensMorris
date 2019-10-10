#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "Player.h"
#include "NineMensMorris.h"


struct cmpGames {
	bool operator()(const NineMensMorris& a, const NineMensMorris& b) const {

		return true;
	}
};


class ComputerPlayer : public Player<NineMensMorris>
{

public:
	std::map<NineMensMorris, int, cmpGames> moves;

	NineMensMorris MiniMax(NineMensMorris game, int player, int depth);

	double heuristics(NineMensMorris game, int id);

	NineMensMorris getBestMove(NineMensMorris game);

	virtual void make_actions(NineMensMorris& game) override;

};