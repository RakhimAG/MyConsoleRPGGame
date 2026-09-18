#include "TraderCivilian.h"

#include <fstream>

TraderCivilian::TraderCivilian(int id, MyString name)
	: Civilian(id, name), _inventory(_INVENTORY_CAPACITY)
{
}

const Inventory& TraderCivilian::get_inventory() const
{
	return _inventory;
}

void TraderCivilian::add_item(const Item* item)
{
	if (!item)		return;

	_inventory.add_item(item);
}

Item* TraderCivilian::sell_item(size_t slot) 
{
	if (slot > _inventory.get_inventory_size())		
	throw std::invalid_argument("sell_item() - \'slot\' argument is more than the actual size of _inventory!\n");

	return _inventory.take_item_out(slot);;
}

void TraderCivilian::buy_item(Player& player, size_t slot)
{
	Item* item =
		player.take_item_out(slot);

	if (!item)
		return;

	_inventory.add_item(item);
}


void TraderCivilian::save(std::ostream& out) const
{
	Civilian::save(out);
	_inventory.save_data(out);
}

void TraderCivilian::load(std::istream& in)
{
	Civilian::load(in);
	_inventory.load_data(in);
}
