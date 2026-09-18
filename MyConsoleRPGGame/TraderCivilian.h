#pragma once
#include "Civilian.h"
#include "Inventory.h"

class TraderCivilian : public Civilian
{
private:

	static constexpr size_t _INVENTORY_CAPACITY = 20;
	Inventory _inventory;
		
public:

	TraderCivilian(int id, MyString name);

	TraderCivilian(const TraderCivilian& other) = default;

	TraderCivilian& operator= (const TraderCivilian& other) = default;

	~TraderCivilian() = default;

	const Inventory& get_inventory() const;

	void add_item(const Item* item);

	// ------------------------------------
	// Sell his Item to Player
	// slot - belongs to Trader's inventory
	// ------------------------------------
	Item* sell_item(size_t slot);

	// ------------------------------------
	// Buy from Player the Item.
	// slot - belongs to Player's inventory
	// -------------------------------------
	void buy_item(Player& player, size_t slot);

	// ======== SAVE | LOAD =========

	void save(std::ostream& out) const override;

	void load(std::istream& in) override;
};

