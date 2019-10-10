#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "Player.h"
#include "NineMensMorris.h"


class ComputerPlayer : public Player<NineMensMorris>
{
	std::map<NineMensMorris, int> moves;
	int MiniMax(NineMensMorris game, int player, int depth);

	int heuristics(NineMensMorris game);

	NineMensMorris getBestMove();

	virtual void make_actions(NineMensMorris& game) override;

};