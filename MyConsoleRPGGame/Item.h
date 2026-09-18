#pragma once
#include "MyString.h"
//#include "GameExecptions.h"

class CombatCharacter;

enum class ItemType
{
	NONE,

	WEAPON,
	ARMOR,
	HEALTH_POTION,
	KEY_ITEM
};

class Item
{
protected:
	ItemType _item_type;
	int _value;
	MyString _name;

public:	

	inline Item(int value, MyString name) : _item_type(ItemType::NONE)
	{
		if (value >= 0)		_value = value;
		else				_value = 0;

		if (name)			_name = name;
		else				_name = "UnknowItem";
	}

	inline Item() : _item_type(ItemType::NONE), _value(0), _name("UnknowItem") {}

	virtual ~Item() = default;

	// ========= MAIN METHODS ==========

	virtual void use(CombatCharacter& user) const = 0;
	
	virtual void on_equip(CombatCharacter& owner) const {}

	virtual void on_unequip(CombatCharacter& owner) const {}

	virtual Item* clone() const = 0;

	// =========== ADDITIONAL METHODS =========

	inline virtual bool is_consumable() const { return false; }

	virtual void render_item_stats() const = 0;

	// =========== GETTERS ============

	inline const MyString& get_name() const { return _name; };

	inline int get_value() const { return _value; }

	virtual ItemType get_item_type() const { return ItemType::NONE; }

	// ========== SAVE | LOAD ==========

	virtual void save(std::ostream& out) const = 0;

	virtual void load(std::istream& in) = 0;

};

