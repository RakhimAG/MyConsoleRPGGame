#include "Weapon.h"
#include "Armor.h"
#include "HealthPotion.h"
#include "KeyItem.h"

#include "Inventory.h"

Inventory::Inventory() : _items(20)
{
}

Inventory::Inventory(size_t inventory_capacity) : _items(inventory_capacity)
{
}

Inventory::Inventory(const Inventory& other)
{
	_items = MyDynamicArray<Item*>(other._items.get_capacity());

	for (size_t i = 0; i < other._items.get_size(); i++)
	{
		_items.push_back(other._items[i]->clone());
	}
}

// ======= CONSTRUCTR ==========
Inventory::Inventory(Inventory&& other) noexcept
{
	this->_items = std::move(other._items);
}

// ======= OPERATORS ============

Item*& Inventory::operator[] (size_t index)
{
	return _items[index];
}

const Item* const& Inventory::operator[] (size_t index) const
{
	return _items[index];
}

Inventory& Inventory::operator=(const Inventory& other)
{
	if (this == &other)
		return *this;

	// clear current items
	for (size_t i = 0; i < _items.get_size(); i++)
	{
		delete _items[i];
	}

	_items.clear();

	// deep copy
	for (size_t i = 0; i < other._items.get_size(); i++)
	{
		_items.push_back(other._items[i]->clone());
	}

	return *this;
}

Inventory& Inventory::operator=(Inventory&& other) noexcept
{
	this->_items = std::move(other._items);
	return *this;
}

// ======== DESTRUCTOR ==========
Inventory::~Inventory()
{
	for (size_t i = 0; i < _items.get_size(); i++)
	{
		delete _items[i];
	}
}

// ======== METHODS =========

// By cloning we are making inventory owner of his items.
void Inventory::add_item(const Item* new_item)
{
	_items.push_back(new_item->clone());
}

void Inventory::destroy_item(size_t slot)
{
	if (slot > _items.get_size())	return;

	delete _items[slot];

	_items.remove_at(slot);
}

Item* Inventory::take_item_out(size_t index)
{
	if (index >= _items.get_size())		return nullptr;

	Item* item = _items[index];

	_items.remove_at(index);

	return item;
}

void Inventory::use_item(size_t slot, CombatCharacter& user)
{
	if (slot >= _items.get_size()) return;

	Item* item = _items[slot];
	
	item->use(user);
	
	if (item->is_consumable())
	{
		destroy_item(slot);
	}
}

void Inventory::use_first(ItemType item_type, CombatCharacter& user)
{
	for (size_t i = 0; i < _items.get_size(); i++)
	{
		if (_items[i]->get_item_type() == item_type)
		{
			_items[i]->use(user);

			if (_items[i]->is_consumable()) destroy_item(i);
			return;
		}
	}
}

void Inventory::clear_items()
{
	for (size_t i = 0; i < _items.get_size(); i++)
	{
		delete _items[i];
	}

	_items.clear();
}

// ========== SAVE | LOAD ============

void Inventory::save_data(std::ostream& out) const
{
	_items.save(out);
}

void Inventory::load_data(std::istream& in)
{
	for (size_t i = 0; i < _items.get_size(); i++)
	{
		delete _items[i];
	}
	_items.clear();

	size_t size = 0;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	

	for (size_t i = 0; i < size; i++)
	{
		ItemType type;
		in.read(reinterpret_cast<char*>(&type), sizeof(type));

		Item* item = nullptr;

		switch (type)
		{
		case ItemType::WEAPON:
			item = new Weapon();
			break;

		case ItemType::ARMOR:
			item = new Armor();
			break;

		case ItemType::HEALTH_POTION:
			item = new HealthPotion();
			break;

		case ItemType::KEY_ITEM:
			item = new KeyItem();
			break;
		}

		item->load(in);
		_items.push_back(item);
	}
}

// ========== GETTERS ==============

const Item* Inventory::get_item(int index) const
{
	if (index < 0 || index >= _items.get_size())
		return nullptr;

	return _items[index];
}

int Inventory::get_item_index(const Item* item) const
{
	for (int i = 0; i < _items.get_size(); i++)
		if (_items[i] == item)
			return i;

	return -1;
}

size_t Inventory::get_inventory_size() const
{
	return _items.get_size();
}
