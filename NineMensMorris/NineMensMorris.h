#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include <windows.h>
#include <iomanip>
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
	using Matrix = std::vector<std::vector<int>>;
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
	enum class ConsoleColor : int
	{
		Black = 0,
		Blue = 1,
		Green = 2,
		Cyan = 3,
		Red = 4,
		Magenta = 5,
		Brown = 6,
		LightGray = 7,
		DarkGray = 8,
		LightBlue = 9,
		LightGreen = 10,
		LightCyan = 11,
		LightRed = 12,
		LightMagenta = 13,
		Yellow = 14,
		White = 15
	} index;
	struct Mill
	{
		int first, second, third;
	};
	
	const static int MENS = 9;
	const static int N = 7;
	const static int UNAVILABLE = -1;
	const static int AVILABLE = 0;

	static std::map<int, std::set<int>> transitions;
	static std::map<int, std::vector<Mill>> mills;
	static std::map<int, ConsoleColor> playerColors;
	
	int players_count;
	int current_player;
	int millsCount;
	int placedCount;
	std::map<PlayerId, int> mens;
	State state, commonState, prevState;
	ActionType prev_from, prev_to;
	Matrix board;
	
	PlayerId nextPlayer();

	bool place(ActionType point);

	bool move(ActionType from, ActionType to);

	bool mill(ActionType point);

	inline void set(ActionType point, PlayerId player)
	{
		board[point.first][point.second] = player;
	}

	inline int get(ActionType point)
	{
		return board[point.first][point.second];
	}

	bool has_trans(ActionType from, ActionType to);

	inline bool can_make_move(ActionType from, ActionType to)
	{
		return belongs(from, current_player) && belongs(to, 0) && (mens[current_player] < 4 || has_trans(from, to));
	}

	void calc_mills(ActionType point);

	inline bool belongs(ActionType point, PlayerId player) {
		return get(point) == player;
	}

	inline bool end()
	{
		return state == State::Draw ||
			state == State::Lose ||
			state == State::Win;
	}

	inline ActionType to_action(int id)
	{
		return { id / N, id % N };
	}

	inline int to_id(ActionType action)
	{
		return action.first * N + action.second;
	}
	
	std::string to_s(State s);

	std::vector<NineMensMorris> getMillBoards();
	
	std::vector<NineMensMorris> getPlaceBoards();
	
	std::vector<NineMensMorris> getMoveBoards();
	
	std::string format(ActionType point, int &cell);

	void print(std::ostream& out, std::vector<NineMensMorris::ConsoleColor> colors);

	std::vector<int> setup(int players_count);

	void SetColor(ConsoleColor text, ConsoleColor background = ConsoleColor::Black);

	NineMensMorris() 
	{
	}
};