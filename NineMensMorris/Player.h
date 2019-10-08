#pragma once

template<typename Game>
class Player
{
public:
	using GameType = Game;
	int id;

	virtual void make_actions(GameType& game) = 0;
};