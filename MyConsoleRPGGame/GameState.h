#pragma once
#include "Game.h"

class Game;

enum Controls
{
	GO_UP = 'w',
	GO_DOWN = 's',
	ENTER = 'e'
};

class GameState
{
protected:
	short _selected_option;

public:
	// -------- DESTRUCTOR --------
	virtual ~GameState() = default;
	// ----------------------------
	
	// -------- CONSTRUCTOR --------
	GameState() : _selected_option(0) {};
	// -----------------------------

	// =================================================================
	// This method will handle every user's input:
	// 1) choosing an option in menu;
	// 2) choosing attack in battle;
	// 3) using items in inventory;
	// 4) choose an option what to do in the map's area;
	// 5) other options like accept quest, or have a dialogue with civilian
	// =================================================================
	virtual void handle_input(Game& game) = 0;

	// =================================================================
	// This method will update game's logic every loop:
	// 1) Should hande enemy's turns in battle;
	// 2) Should do calculations and check for win||lose conditions;
	// 3) Updating dialogue lines;
	// Overall - run the game rules.
	// =================================================================
	virtual void update(Game& game) = 0;

	// =================================================================
	// This method will 'render' visuals with ASCII characters:
	// 1) All the menu's and the main menu;
	// 2) Should be responsible for coloring;
	// 3) Rendering stats in battle;
	// 4) Rendering enemy's body in battle;
	// 5) Rendering trader's sentry stations;
	// 6) Optional rendering Inventory;
	// =================================================================
	virtual void render(const Game& game) const = 0;

};


// =============================
// Unicode broke my code so no use.
// https://patorjk.com/software/taag/#p=display&f=Classy&t=Welcome&x=none&v=4&h=4&w=80&we=false = TEXT -> ASCII (FONT - Classy)
// ===============================