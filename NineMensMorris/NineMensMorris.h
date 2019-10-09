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

	using Matrix = std::vector<std::vector<int>>;
	using PlayerId = int;

	Matrix board;

	int millsCount;
	int placedCount;

	std::map<PlayerId, int> mens;
	
	State state, commonState, prevState;
		
	ActionType prev_from, prev_to;
	
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
	static int players_count;

	bool require_action();
	
	void nextPlayer(PlayerId player);

	bool place(ActionType point, PlayerId player);

	bool move(ActionType from, ActionType to, PlayerId player);

	bool mill(ActionType point, PlayerId player);

	inline void set(ActionType point, PlayerId player)
	{
		board[point.first][point.second] = player;
	}

	inline int get(ActionType point)
	{
		return board[point.first][point.second];
	}

	bool has_trans(ActionType from, ActionType to);

	inline bool can_make_move(ActionType from, ActionType to, PlayerId player)
	{
		return belongs(from, player) && belongs(to, 0) && (mens[player] < 4 || has_trans(from, to));
	}

	void calc_mills(ActionType point, PlayerId player);

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

	//getMillActions getPlaceActions getMoveActions

	std::vector<NineMensMorris> getMillBoards(NineMensMorris board, PlayerId player);
	
	std::vector<NineMensMorris> getPlaceBoards(NineMensMorris board, PlayerId player);
	
	std::vector<NineMensMorris> getMoveBoards(NineMensMorris board, PlayerId player);
	
	std::string format(ActionType point);

	void print(std::ostream& out);

	void setup(int players_count);

	NineMensMorris() 
	{
	}
};