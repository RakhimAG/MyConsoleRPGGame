#pragma once
#include "Item.h"

enum class ArmorType
{
	NONE,

	HELMET,
	CHESTPLATE,
	BOOTS
};

class CombatCharacter;

class Armor : public Item
{
private:
	int _defense_points;
	ArmorType _armor_type;
	
public:
	Armor() :Item(), _defense_points(0), _armor_type(ArmorType::NONE) {}

	Armor(
		int value, MyString name,
		int defense_points, ArmorType armor_type
	);

	Armor(const Armor& other) = default;

	Armor& operator= (const Armor& other) = default;

	// ======== MAIN METHODS ========

	void use(CombatCharacter& user) const override;

	void on_equip(CombatCharacter& owner) const override;

	void on_unequip(CombatCharacter& owner) const override;

	Item* clone() const override;

	// ========== GETTERS ===========

	void render_item_stats() const override;

	inline ItemType get_item_type() const override { return ItemType::ARMOR; }

	ArmorType get_armor_type() const;

	inline int get_defense_points() const { return _defense_points; }

	// ========== SAVE | LOAD ===========

	void save(std::ostream& out) const override;

	void load(std::istream& in) override;
};

