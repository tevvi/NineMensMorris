#pragma once
#include <vector>
#include <ostream>

template<typename Board, typename Action>
class GameHandler
{
public:
	using BoardType = Board;
	using ActionType = Action;
	using PlayerId = int;
	BoardType board;

	virtual void print(std::ostream& out) = 0;
	virtual bool end() = 0;
};