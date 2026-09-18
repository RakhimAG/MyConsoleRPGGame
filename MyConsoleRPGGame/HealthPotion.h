#pragma once
#include "Item.h"

class CombatCharacter;

class HealthPotion : public Item
{
private:
	int _healing_points;

public:
	HealthPotion() : Item(), _healing_points(0) {}
	
	~HealthPotion() = default;

	HealthPotion(const HealthPotion& other) = default;

	HealthPotion
	(
		int value, MyString name,
		int healing_points
	);

	// ======== MAIN METHODS ========

	void use(CombatCharacter& user) const override;

	Item* clone() const override;

	void render_item_stats() const override;

	inline bool is_consumable() const override { return true; }

	inline ItemType get_item_type() const override { return ItemType::HEALTH_POTION; }

	// ========= SAVE | LOAD =============

	void save(std::ostream& out) const override;

	void load(std::istream& in) override;
};

