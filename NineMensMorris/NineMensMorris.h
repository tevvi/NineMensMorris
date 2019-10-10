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





//Основной класс, реализующий логику игры
class NineMensMorris
{
public:
	using ActionType = std::pair<int, int>;
	using PlayerId = int;
	using Matrix = std::vector<std::vector<int>>;
	using PlayerId = int;
	
	//Состояние игры
	enum class State
	{
		Lose = 0,
		Win = 1,		//Кем-то из игроков одержана победа
		Draw = 2,
		Placing,		//Ожидает от игрока выставления одной фишки
		Moving,			//Ожитает от игрока перемещения фишк
		Mill,			//Ожидает, что игрок заберет фишку противника
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
	const static int MAX_DEPTH = 4;

	static std::map<int, std::set<int>> transitions;
	static std::map<int, std::vector<Mill>> mills;
	static std::map<int, ConsoleColor> playerColors;
	
	int players_count;
	int current_player;
	int millsCount;
	int placedCount;
	std::map<PlayerId, int> mens;	//Словарь, содержащий данные о том, сколько фишек осталось у игрока с заданным id
	State state, commonState, prevState;	//state, commonState - локальное и глобальное текущие состояния. prevState - состояние, из которого мы пришли в текущее
	ActionType prev_from, prev_to; //Данные о фишке, использованной в предыдущем ходе. Если prevState = Mill или Place, то в prev_to лежит позиция убранной/добавленной фишки. Если Move, то в prev_from, prev_to лежит, откуда и куда фишка была перемещена
	Matrix board;	//матрица, содержащая расположение фишек на игровом поле
	
	PlayerId nextPlayer();

	//Поместить фишку текущего игрока на позицию point
	bool place(ActionType point);

	//Передвинуть фишку текущего игрока с позиции from на позицию to
	bool move(ActionType from, ActionType to);

	//Убрать фишку противника с позиции point
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

	//Закончена ли игра
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

	//Возвращает все игры, которые можно получить из текущей, если игрок заберет у противника одну фишку
	std::vector<NineMensMorris> getMillBoards();
	
	//Возвращает все игры, которые можно получить из текущей, если игрок разместит фишку на поле
	std::vector<NineMensMorris> getPlaceBoards();
	
	//Возвращает все игры, которые можно получить из текущей, если игрок передвинет фишку
	std::vector<NineMensMorris> getMoveBoards();
	
	std::string format(ActionType point, int &cell);

	void print(std::ostream& out, std::vector<NineMensMorris::ConsoleColor> colors);

	std::vector<int> setup(int players_count);

	void SetColor(ConsoleColor text, ConsoleColor background = ConsoleColor::Black);

	
	NineMensMorris() 
	{
	}
};
