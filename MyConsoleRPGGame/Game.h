#pragma once

// ENTITY : PLAYER
#include "Player.h"

// WORLD
#include "Area.h"

// GAMESTATE
#include "GameState.h"

// CONTAINERS
#include "MyDynamicArray.h"

class GameState;

class Player;

class Game
{
private:
	bool _is_running;

	bool  _state_change_requested;

	Player _player;

	// --------------------------------------
	// _locations own _current_area so -
	// it will be responsible for deleting it
	// ---------------------------------------
	MyDynamicArray<Area*> _locations;
	Area* _current_area;

	GameState* _current_state;
	GameState* _next_state;

	static size_t _STARTER_AREA_INDEX;

	static constexpr size_t _MAX_LOCATIONS = 4;

	static constexpr size_t VILLAGE = 0;
	static constexpr size_t FOREST = 1;
	static constexpr size_t CAVE = 2;
	static constexpr size_t CAMP = 3;

public:
	Game();

	~Game();

	void set_current_area(Area* other_area);

	// should be pointer - for deleting old state correctly
	void switch_state(GameState* new_state);

	// run the game loop
	void run();

	// exit the game loop
	void quit();

	// ======== SAVE | LOAD =========

	void save(std::ostream& out) const;

	void load(std::istream& in);

	// ======== GETTERS =========

	Player& get_player();
	const Player& get_player() const;

	MyDynamicArray<Area*>& get_locations();
	const MyDynamicArray<Area*>& get_locations() const;

	const Area* get_area(size_t index) const;
	Area* get_area(size_t index);

	int get_area_index(Area* area) const;

	const Area* get_current_area() const;

	Area* get_current_area();

	const Area* get_starting_area() const;
	Area* get_starting_area();

	// ======== HELPERS =========

	Quest* find_quest_by_id(int id);

private:

	void apply_state_change();

	// ============ METHODS FOR INITIALIZING EVERY OBJECT ==================

	// main method for initializing every object by default values
	void init();

	void InitializePlayer();

	void InitializeAreas();

	void InitializeCivilians();

	void InitializeEnemies();

	void ConnectAreas();

	void InitializeHiddenItems();
};



