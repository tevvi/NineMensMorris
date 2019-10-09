#include "NineMensMorris.h"

void NineMensMorris::SetColor(ConsoleColor text, ConsoleColor background)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		(WORD)((static_cast<int>(background) << 4) | static_cast<int>(text)));
}

std::map<int, std::set<int>> NineMensMorris::transitions;
std::map<int, std::vector<NineMensMorris::Mill>> NineMensMorris::mills;
std::map<int, NineMensMorris::ConsoleColor> NineMensMorris::playerColors;
int NineMensMorris::players_count;

bool NineMensMorris::require_action()
{
	return state == State::Mill ||
		state == State::Moving ||
		state == State::Placing;
}

void NineMensMorris::nextPlayer(PlayerId player) {
	state = commonState;
}

bool NineMensMorris::place(ActionType point, PlayerId player)
{
	if (!belongs(point, 0)) {
		return false;
	}
	set(point, player);
	mens[player]++;
	placedCount++;
	if (placedCount == players_count * MENS) {
		commonState = State::Moving;
	}
	state = State::NextPlayer;
	calc_mills(point, player);
	return true;
}

bool NineMensMorris::move(ActionType from, ActionType to, PlayerId player)
{
	if (!can_make_move(from, to, player)) {
		return false;
	}
	set(from, 0);
	set(to, player);
	state = State::NextPlayer;
	calc_mills(to, player);
	return true;
}

bool NineMensMorris::mill(ActionType point, PlayerId player)
{
	if (belongs(point, player) || belongs(point, 0) || belongs(point, -1))
	{
		return false;
	}
	mens[board[point.first][point.second]]--;
	set(point, 0);
	millsCount--;
	if (millsCount == 0)
		state = State::NextPlayer;
	if (mens[get(point)] == 2 && commonState != State::Placing) {
		state = State::Win;
	}
	return true;
}

bool NineMensMorris::has_trans(ActionType from, ActionType to)
{
	auto& set = transitions[to_id(from)];
	return set.find(to_id(to)) != set.end();
}

void NineMensMorris::calc_mills(ActionType point, PlayerId player)
{
	for (auto mill : mills[to_id(point)]) {
		if (belongs(to_action(mill.first), player) &&
			belongs(to_action(mill.second), player) &&
			belongs(to_action(mill.third), player))
		{
			millsCount++;
		}
	}
	if (millsCount > 0) {
		state = State::Mill;
	}
}

std::vector<NineMensMorris> NineMensMorris::getMillBoards(NineMensMorris board, PlayerId player)
{
	std::vector<NineMensMorris> res = {};
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			if (!board.belongs({ i, j }, player) && !board.belongs({ i, j }, 0) && !board.belongs({ i, j }, -1))
			{
				NineMensMorris game = board;
				game.mill({ i, j }, player);
				game.prevState = State::Mill;
				game.prev_to = { i ,j };
				res.push_back(game);
			}
		}
	}

	return res;
}

std::vector<NineMensMorris> NineMensMorris::getPlaceBoards(NineMensMorris board, PlayerId player)
{
	std::vector<NineMensMorris> res = {};
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (board.belongs({ i, j }, 0))
			{
				NineMensMorris game = board;
				game.place({ i, j }, player);
				game.prevState = State::Placing;
				game.prev_to = { i, j };
				res.push_back(game);
			}
		}
	}
	return res;
}

std::vector<NineMensMorris> NineMensMorris::getMoveBoards(NineMensMorris board, PlayerId player)
{
	std::vector<NineMensMorris> res = {};
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (board.belongs({ i, j }, player))
			{
				for (auto trans : transitions[to_id({ i, j })]) {
					ActionType trans_act = to_action(trans);
					if (board.belongs(trans_act, 0)) {
						NineMensMorris game = board;
						game.move({ i, j }, trans_act, player);
						game.prevState = State::Moving;
						game.prev_from = { i, j };
						game.prev_to = trans_act;
						res.push_back(game);
					}
				}
			}
		}
	}
	return res;
}

/*std::string NineMensMorris::format(ActionType point)
{
	if (belongs(point, -1))
		return " ";
	if (belongs(point, 0))
		return "0";
	SetColor(playerColors[get(point)]);
	return std::to_string(get(point));
}*/

std::string NineMensMorris::format(ActionType point, int& cell)
{
	if (get(point) == -1) 
	{
		return "  ";
	}
	else if (get(point) != 0)
	{
		SetColor(playerColors[get(point)]);
	}
	std::string res = cell / 10 == 0 ? std::to_string(cell) + " " : std::to_string(cell);
	cell++;
	return res;
}

void NineMensMorris::print(std::ostream& out)
{
	int cell = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			SetColor(ConsoleColor::White);
			out << format({ i, j }, cell) << " ";
		}
		out << std::endl;
	}
	SetColor(ConsoleColor::White);
}

void NineMensMorris::setup(int players_count)
{
	state = State::Placing;
	this->players_count = players_count;
	mens = std::map<PlayerId, int>();
	for (int i = 0; i < players_count; i++)
	{
		mens[i + 1] = 0;
	}

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
	placedCount = 0;
	millsCount = 0;

	for (int i = 0; i < players_count; i++) {
		playerColors[i + 1] = (ConsoleColor)(i + 3);
	}
	commonState = State::Placing;
}



