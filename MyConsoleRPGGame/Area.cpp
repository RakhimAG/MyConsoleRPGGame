#include "Area.h"

#include "GameExecptions.h"

// FOR CHOOSING RANDOM ENEMY INSIDE _enemy_pool
#include "RandInt.h"

Area::Area(const MyString& name, const MyString& description) :
	_civilians(_MAX_CIVILIANS_CAPACITY),
	_enemy_pool(_MAX_ENEMIES_CAPACITY),
	_connected_areas(_MAX_CONNECTABLE_AREAS),
	_collectable_items(_MAX_ITEMS_CAPACITY)
{
	if (name)		_name = name;
	else			_name = "NoNameArea";

	if (description)	_description = description;
	else				_description = "No Description";
}

Area::~Area()
{
	for (size_t i = 0; i < _enemy_pool.get_size(); i++)
	{
		delete _enemy_pool[i];
	}
	
	for (size_t i = 0; i < _collectable_items.get_size(); i++)
	{
		delete _collectable_items[i];
	}
}

// ------------------------------------------------------------------
// ARGUMENT SHOULD BE PASSED LIKE THIS : new Enemy()
// Otherwise there is risk that this object might not outlive Area.
// ------------------------------------------------------------------
void Area::add_enemy(Enemy* enemy)
{
	_enemy_pool.push_back(enemy);
}

void Area::add_civilian(Civilian* civilian)
{
	_civilians.push_back(civilian);
}

void Area::add_hidden_item(Item* item)
{
	_collectable_items.push_back(item);
}

// ------------------------------------------------------------------
// ARGUMENT SHOULD BE PASSED LIKE THIS : new Area()
// Otherwise there is risk that this object might not outlive Area.
// ------------------------------------------------------------------
void Area::connect_area(Area* area)
{
	_connected_areas.push_back(area);
}
// ============== SAVE | LOAD ===============

void Area::save(std::ostream& out) const
{
	// other members are constant and not dynamic during Run Time
	// they don't require saving

	_enemy_pool.save(out);

	_civilians.save(out);
}

void Area::load(std::istream& in)
{
	// enemies
	size_t enemies_size = 0;
	in.read(reinterpret_cast<char*>(&enemies_size), sizeof(enemies_size));

	if (enemies_size != _enemy_pool.get_size())
		throw FileOperationException(MyString("Area load failed due to corrupted size of _civilians!"));

	for (size_t i = 0; i < enemies_size; i++)
	{
		_enemy_pool[i]->load(in);
	}

	// civilians
	size_t civilians_size = 0;
	in.read(reinterpret_cast<char*>(&civilians_size), sizeof(civilians_size));

	if (civilians_size != _civilians.get_size())
		throw FileOperationException(MyString("Area load failed due to corrupted size of _civilians!"));

	for (size_t i = 0; i < civilians_size; i++)
	{
		_civilians[i]->load(in);
	}
}

// =============================================

const MyDynamicArray<Civilian*>& Area::get_civilians() const
{
	return _civilians;
}

const MyDynamicArray<Item*>& Area::get_hidden_items() const
{
	return _collectable_items;
}

const MyDynamicArray<Enemy*>& Area::get_enemy_pool() const
{
	return _enemy_pool;
}

const MyDynamicArray<Area*>& Area::get_connected_areas() const
{
	return _connected_areas;
}

const Enemy* Area::choose_next_enemy() const
{
	if (_enemy_pool.get_size() == 0)		return nullptr;

	for (size_t i = 0; i < _enemy_pool.get_size(); i++)
	{
		if (_enemy_pool[i]->is_alive() == true)
			return _enemy_pool[i];
	}

	return nullptr;
}

Enemy* Area::choose_next_enemy()
{
	if (_enemy_pool.get_size() == 0)		return nullptr;

	for (size_t i = 0; i < _enemy_pool.get_size(); i++)
	{
		if (_enemy_pool[i]->is_alive() == true)
			return _enemy_pool[i];
	}

	return nullptr;
}

const Item* Area::get_random_item() const
{
	if (_collectable_items.get_size() == 0)		return nullptr;

	size_t last_item_index = _collectable_items.get_size() - 1;
	size_t random_index = static_cast<size_t>(RandInt(0, last_item_index));

	return _collectable_items[random_index];
}

const MyString& Area::get_description() const
{
	return _description;
}

const MyString& Area::get_name() const
{
	return _name;
}


