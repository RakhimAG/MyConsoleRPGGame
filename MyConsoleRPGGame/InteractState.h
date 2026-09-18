#pragma once
#include "GameState.h"


class InteractState : public GameState
{
	Civilian* _civilian;
public:

	InteractState(Civilian* civilian);

	void handle_input(Game& game) override;

	void update(Game& game) override;

	void render(const Game& game) const override;
};

