#include "HumanPlayer.h"
#include "NineMensMorris.h"

using Game = NineMensMorris;
using GameState = typename Game::State;

//Процесс игры
void play(Game&  game, std::vector<Player<Game>* > players, std::vector<NineMensMorris::ConsoleColor> colors)
{
	//инициализируем игру
	auto players_id = game.setup(2);
	while (!game.end())
	{
		game.nextPlayer();
		game.print(std::cout, colors);
		players[game.current_player - 1]->make_actions(game);	//Ждем действий текущего игрока и выполняем их
		std::cout << std::endl << std::endl;
	}
	std::cout << "Player " << game.current_player << " " << game.to_s(game.state) << " !\n";
	system("PAUSE");
}

int main()
{
	NineMensMorris game;
	HumanPlayer p1, p2;
	std::vector<NineMensMorris::ConsoleColor> colors = {NineMensMorris::ConsoleColor::Yellow, NineMensMorris::ConsoleColor::Blue };

	play(game, {&p1, &p2 }, colors);
	
}