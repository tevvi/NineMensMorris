#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>

#include "GameHandler.h"



namespace base
{
	class NotImplementedException : public std::logic_error
	{
	public:
		NotImplementedException() : std::logic_error{ "Function not yet implemented." } {}
	};
}

class NineMensMorris
{
public:
	using ActionType = std::pair<int, int>;
	using PlayerId = int;
	enum class State
	{
		Lose = 0,
		Win = 1,
		Draw = 2,
		Placing,
		Moving,
		Mill,
		NextPlayer
	};

	struct Board
	{
		using Matrix = std::vector<std::vector<int>>;
		using ActionType = std::pair<int, int>;
		using PlayerId = int;
		Matrix board;
		int millsCount;
		int placedCount;
		std::map<PlayerId, int> mens;

		State state, commonState;
		
		ActionType prev_from, prev_to;
	};
	using BoardType = Board;
	
	struct Mill
	{
		int first, second, third;
	};

	const static int MENS = 9;
	const static int N = 7;
	const static int UNAVILABLE = -1;
	const static int AVILABLE = 0;

	std::map<int, std::set<int>> transitions;
	std::map<int, std::vector<Mill>> mills;
	int players_count;
	BoardType board;

	bool require_action()
	{
		return board.state == State::Mill ||
			board.state == State::Moving ||
			board.state == State::Placing;
	}
	
	void nextPlayer(PlayerId player) {
		board.state = board.commonState;
	}

	bool place(ActionType point, PlayerId player)
	{
		if (!belongs(point, 0)) {
			return false;
		}
		set(point, player);
		board.mens[player]++;
		board.placedCount++;
		if (board.placedCount == players_count * MENS) {
			board.commonState = State::Moving;
		}
		board.state = State::NextPlayer;
		calc_mills(point, player);
		return true;
	}

	bool move(ActionType from, ActionType to, PlayerId player)
	{
		if (!can_make_move(from, to, player)) {
			return false;
		}
		set(from, 0);
		set(to, player);
		board.state = State::NextPlayer;
		calc_mills(to, player);
		return true;
	}

	bool mill(ActionType point, PlayerId player)
	{
		if (belongs(point, player) || belongs(point, 0) || belongs(point, -1))
		{
			return false;
		}
		board.mens[board.board[point.first][point.second]]--;
		set(point, 0);
		board.millsCount--;
		if (board.millsCount == 0)
			board.state = State::NextPlayer;
		if (board.mens[get(point)] == 2 && board.commonState != State::Placing) {
			board.state = State::Win;
		}
		return true;
	}

	void set(ActionType point, PlayerId player)
	{
		board.board[point.first][point.second] = player;
	}

	int get(ActionType point)
	{
		return board.board[point.first][point.second];
	}

	bool has_trans(ActionType from, ActionType to)
	{
		auto& set = transitions[to_id(from)];
		return set.find(to_id(to)) != set.end();
	}

	bool can_make_move(ActionType from, ActionType to, PlayerId player)
	{
		return belongs(from, player) && belongs(to, 0) && (board.mens[player] < 4 || has_trans(from, to));
	}

	void calc_mills(ActionType point, PlayerId player)
	{
		for (auto mill : mills[to_id(point)]) {
			if (belongs(to_action(mill.first), player) &&
				belongs(to_action(mill.second), player) &&
				belongs(to_action(mill.third), player))
			{
				board.millsCount++;
			}
		}
		if (board.millsCount > 0) {
			board.state = State::Mill;
		}
	}

	bool belongs(ActionType point, PlayerId player) {
		return get(point) == player;
	}

	bool end()
	{
		return board.state == State::Draw ||
			board.state == State::Lose ||
			board.state == State::Win;
	}

	ActionType to_action(int id)
	{
		return { id / N, id % N };
	}

	int to_id(ActionType action)
	{
		return action.first * N + action.second;
	}

	//getMillActions getPlaceActions getMoveActions

	std::vector<BoardType> getMillBoards(BoardType board, PlayerId player)
	{

	}
	
	std::vector<BoardType> getPlaceBoards(BoardType board, PlayerId player)
	{

	}
	
	std::vector<BoardType> getMoveBoards(BoardType board, PlayerId player)
	{

	}
	
	std::string format(ActionType point)
	{
		if (belongs(point, -1))
			return " ";
		if (belongs(point, 0))
			return "0";
		return std::to_string(get(point));
	}

	void print(std::ostream& out)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				out << format({i, j}) << " ";
			}
			out << std::endl;
		}
	}

	void setup(int players_count)
	{
		board.state = State::Placing;
		this->players_count = players_count;
		board.mens = std::map<PlayerId, int>();
		for (int i = 0; i < players_count; i++)
		{
			board.mens[i + 1] = 0;
		}

		board.board = {
			{  0, -1, -1,  0, -1, -1,  0 },
			{ -1,  0, -1,  0, -1,  0, -1 },
			{ -1, -1,  0,  0,  0, -1, -1 },
			{  0,  0,  0, -1,  0,  0,  0 },
			{ -1, -1,  0,  0,  0, -1, -1 },
			{ -1,  0, -1,  0, -1,  0, -1 },
			{  0, -1, -1,  0, -1, -1,  0 }
		};
		/*
		00----03----06
		--08--10--12--
		----161718----
		212223--252627
		----303132----
		--36--38--40--
		42----45----48
		*/
		mills[0] = { {0, 3, 6}, {0, 21, 42} };
		mills[3] = { {3, 10, 17}, {0, 3, 6} };
		mills[6] = { {0, 3, 6}, {6, 27, 48} };
		mills[8] = { {8, 10, 12}, {8, 22, 36} };
		mills[10] = { {3, 10, 17}, {8, 10, 12} };
		mills[12] = { {8, 10, 12}, {12, 26, 40} };
		mills[16] = { {16, 17, 18}, {16, 23, 30} };
		mills[17] = { {16, 17, 18}, {3, 10, 17} };
		mills[18] = { {16, 17, 18}, {18, 25, 32} };
		mills[21] = { {21, 22, 23}, {0, 21, 42} };
		mills[22] = { {21, 22, 23}, {8, 22, 36} };
		mills[23] = { {21, 22, 23}, {16, 23, 30} };
		mills[25] = { {25, 26, 27}, {18, 25, 32} };
		mills[26] = { {25, 26, 27}, {12, 26, 40} };
		mills[27] = { {25, 26, 27}, {6, 27, 48} };
		mills[30] = { {30, 31, 32}, {16, 23, 30} };
		mills[31] = { {30, 31, 32}, {31, 38, 45} };
		mills[32] = { {30, 31, 32}, {18, 25, 32} };
		mills[36] = { {36, 38, 40}, {8, 22, 36} };
		mills[38] = { {36, 38, 40}, {31, 38, 45} };
		mills[40] = { {36, 38, 40}, {12, 26, 40} };
		mills[42] = { {42, 45, 48}, {0, 21, 42} };
		mills[45] = { {42, 45, 48}, {31, 38, 45} };
		mills[48] = { {42, 45, 48}, {6, 27, 48} };

		transitions[0] = { 21, 3 };
		transitions[3] = { 0, 6, 10 };
		transitions[6] = { 3, 27 };
		transitions[8] = { 10, 22 };
		transitions[10] = { 8, 3, 12, 17 };
		transitions[12] = { 10, 26 };
		transitions[16] = { 23, 17 };
		transitions[17] = { 16, 10, 18 };
		transitions[18] = { 17, 25 };
		transitions[21] = { 0, 42, 22 };
		transitions[22] = { 21, 8, 23, 36 };
		transitions[23] = { 22, 16, 30 };
		transitions[25] = { 18, 26, 32 };
		transitions[26] = { 12, 25, 27, 40 };
		transitions[27] = { 26, 6, 48 };
		transitions[30] = { 23, 31 };
		transitions[31] = { 30, 38, 32 };
		transitions[32] = { 25, 31 };
		transitions[36] = { 22, 38 };
		transitions[38] = { 36, 31, 40, 45 };
		transitions[40] = { 38, 26 };
		transitions[42] = { 21, 45 };
		transitions[45] = { 42, 38, 48 };
		transitions[48] = { 45, 27 };
		board.placedCount = 0;
		board.millsCount = 0;
		board.commonState = State::Placing;
	}

	NineMensMorris() {
	}
};