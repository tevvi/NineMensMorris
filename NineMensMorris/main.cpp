#include "HumanPlayer.h"
#include "NineMensMorris.h"

using Game = NineMensMorris;
using GameState = typename Game::State;

void play(Game&  game, std::vector<Player<Game>* > players, std::vector<NineMensMorris::ConsoleColor> colors)
{
	auto players_id = game.setup(2);
	game.print(std::cout);
	while (!game.end())
	{
		game.nextPlayer();
		game.print(std::cout, colors);
		players[game.current_player - 1]->make_actions(game);
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