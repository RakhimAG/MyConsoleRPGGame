#pragma once
#include "MyDynamicArray.h"
#include "Item.h"

class CombatCharacter;

class Inventory
{
private:

	MyDynamicArray<Item*> _items;

public:
	// -------- CONSTRUCTORS ---------
	Inventory();

	Inventory(size_t inventory_capacity);

	Inventory(const Inventory& other);

	Inventory(Inventory&& other) noexcept;

	// --------- DESTRUCTOR ---------
	~Inventory();

	// ------- OPERATORS ---------

	Item*& operator[] (size_t index);

	const Item* const& operator[] (size_t index) const;

	Inventory& operator= (const Inventory& other);

	Inventory& operator= (Inventory&& other) noexcept;

	// ------- METHODS ---------

	void add_item(const Item* new_item);
	
	void destroy_item(size_t slot);

	Item* take_item_out(size_t slot);

	void use_item(size_t slot, CombatCharacter& user);

	// ------------------------------------------
	// Use first item that is met inside array.
	// Searching will be by its type.
	// ------------------------------------------
	void use_first(ItemType item_type, CombatCharacter& user);

	void clear_items();

	// ======== SAVING TO FILE ==========
	
	void save_data(std::ostream& out) const;

	void load_data(std::istream& in);

	// -------- GETTERS --------

	const Item* get_item(int index) const;

	int get_item_index(const Item* item) const;

	size_t get_inventory_size() const;
};

