#pragma once
#include "Item.h"

enum class WeaponType
{
	NONE,

	SWORD,
	GREAT_AXE,
	CLUB
};

class Weapon : public Item
{
private:
	int _weapon_damage;
	WeaponType _weapon_type;

public:

	Weapon() : Item(), _weapon_damage(0), _weapon_type(WeaponType::NONE) {}

	Weapon(
		int value, MyString name,
		int weapon_damage, WeaponType weapon_type	
		);

	Weapon(const Weapon& other) = default;

	~Weapon() = default;

	// ======== MAIN METHODS ========

	void use(CombatCharacter& user) const override;

	void on_equip(CombatCharacter& owner) const override;

	void on_unequip(CombatCharacter& owner) const override;

	// ========= ADDITIONAL METHODS ========

	Item* clone() const override;

	void render_item_stats() const override;

	inline ItemType get_item_type() const override { return ItemType::WEAPON; }

	WeaponType get_weapon_type() const;

	inline int get_damage_points() const { return _weapon_damage; }

	// ======== SAVE | LOAD =========

	void save(std::ostream& out) const override;

	void load(std::istream& in) override;
};


