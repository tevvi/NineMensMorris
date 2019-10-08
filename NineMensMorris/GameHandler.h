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

	virtual std::vector<ActionType> actions(PlayerId player) = 0;
	virtual std::vector<BoardType> boards(BoardType board, PlayerId player) = 0;
	virtual void print(std::ostream& out) = 0;
	virtual bool end() = 0;
};