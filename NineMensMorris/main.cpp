#include "HumanPlayer.h"
#include "NineMensMorris.h"

using Game = NineMensMorris;
using GameState = typename Game::State;

void play(Game&  game, std::vector<Player<Game>* > players)
{
	game.setup(2);
	game.print(std::cout);
	while (!game.end())
	{
		for (int i = 0; i < players.size(); i++)
		{
			game.nextPlayer(players[i]->id);
			players[i]->make_actions(game);
			game.print(std::cout);
			std::cout << std::endl << std::endl;
			if (game.end())
			{
				if (game.state == NineMensMorris::State::Win) {
					std::cout << "Player " << i << " won!\n";
				}
				break;
			}
		}
	}
}

int main()
{
	NineMensMorris game;
	HumanPlayer p1, p2;
	p1.id = 1;
	p2.id = 2;

	play(game, {&p1, &p2 });
	system("PAUSE");
}