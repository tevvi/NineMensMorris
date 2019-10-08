#pragma once
#include <map>
#include <set>
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


class NineMensMorris : public GameHandler<std::vector<std::vector<int>>, std::pair<int, int>>
{
public:
	using GameHandlerType = GameHandler<std::vector<std::vector<int>>, std::pair<int, int>>;
	using typename GameHandlerType::ActionType;
	using typename GameHandlerType::BoardType;
	using ValueType = typename BoardType::value_type;
	using typename GameHandlerType::PlayerId;
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

	const int MENS = 9;
	const int N = 7;
	const int UNAVILABLE = -1;
	const int AVILABLE = 0;
	std::map<int, std::set<int>> transitions;
	std::vector<int> players_id;

	State state;

	virtual std::vector<ActionType> actions(PlayerId player) override
	{
		switch (state)
		{
		case State::Placing:
			break;
		case State::Moving:
			break;
		case State::Mill:
			break;
		default:
			break;
		}
		throw base::NotImplementedException();
	}
	void place(ActionType point, PlayerId player)
	{
		board[point.first][point.second] = player;
	}


	virtual std::vector<BoardType> boards(BoardType board, PlayerId player) override
	{
		throw base::NotImplementedException(); 
	}
	virtual void print(std::ostream& out) override
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				out << std::to_string(board[i][j])  << "  ";
			}
			out << std::endl;
		}
		//throw base::NotImplementedException();
	}

	virtual bool end() override
	{
		return state == State::Draw ||
			state == State::Lose ||
			state == State::Win;
	}

	ActionType to_action(int id)
	{
		return { id / N, id % N };
	}
	int to_id(ActionType action)
	{
		return action.first * N + action.second;
	}

	void setup(int players_count)
	{
		state = State::Placing;
		players_id = std::vector<int>(players_count);
		for (int i = 0; i < players_count; i++)
			players_id[i] = i + 1;

		board = {
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
	}

	NineMensMorris() {
	}
};