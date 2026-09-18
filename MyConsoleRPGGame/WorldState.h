#pragma once
#include "GameState.h"

class WorldState : public GameState
{
private:
	enum AreaOptions
	{
		EXPLORE,
		TALK,
		TRAVEL
	};

	enum WorldMode
	{
		MENU,
		EXPLORE_EVENT,
		TALK_EVENT,
		TRAVEL_EVENT
	};

	enum ExploreEvents
	{
		NONE,

		ENCOUNTER_ENEMY,
		FIND_AN_ITEM
	};

	WorldMode _mode;
	ExploreEvents _explore_event;

	const Item* _event_item;

public:

	// =========== CONSTRUCTORS =========
	
	WorldState() : _mode(MENU), _explore_event(NONE), _event_item(nullptr) {}

	WorldState(const WorldState& other) = delete;

	WorldState(WorldState&& other) = delete;

	// =========== DESTRUCTOR ===========

	~WorldState() = default;

	// =========== OPERATORS ===========

	WorldState& operator =(const WorldState& other) = default;

	WorldState& operator =(WorldState&& other) = delete;

	// =========== MAIN METHODS ===========

	void handle_input(Game& game) override;

	void update(Game& game) override;

	void render(const Game& game) const override;
	
private:

	void render_explore(const Game& game) const;

	void render_inhabitants(const Game& game) const;
	
	void render_travel(const Game& game) const;
};

