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

	int MiniMax(NineMensMorris game, int player, int depth);

	int heuristics(NineMensMorris game);

	NineMensMorris getBestMove();

	virtual void make_actions(NineMensMorris& game) override;

};