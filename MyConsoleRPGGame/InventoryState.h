#pragma once
#include "GameState.h"

class InventoryState : public GameState
{
private:

	enum ItemOptions
	{
		USE,
		DROP,

		GO_BACK
	};

	enum RenderMode
	{
		INVENTORY_DATA,
		ITEM_DATA
	};
	
	RenderMode _mode;
	const Item* _current_item;
	size_t _item_index;
public:

	InventoryState() : _mode(INVENTORY_DATA), _current_item(nullptr), _item_index(0) {}

	void handle_input(Game& game) override;

	void update(Game& game) override;

	void render(const Game& game) const override;

private:

	void render_inventory(const Inventory& inventory) const;

	void render_item_menu() const;
};

