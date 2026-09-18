#pragma once
#include "GameState.h"

class MainMenuState : public GameState
{
public:
	~MainMenuState() = default;

	MainMenuState() = default;

	MainMenuState(const MainMenuState& other) = delete;

	MainMenuState(MainMenuState&& other) = delete;

	MainMenuState& operator =(const MainMenuState& other) = default;

	MainMenuState& operator =(MainMenuState&& other) = delete;

	void handle_input(Game& game) override;

	void update(Game& game) override;

	void render(const Game& game) const override;
};

