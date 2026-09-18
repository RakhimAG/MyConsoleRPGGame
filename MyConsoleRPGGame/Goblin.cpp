#include "Goblin.h"

Goblin::Goblin(
	int id, const MyString& name,
	int health, int max_health, int damage, int defense,
	int exp_reward, int gold_loot
) : Enemy(
	id, name, health,
	max_health, damage, defense,
	EnemyType::GOBLIN, exp_reward, gold_loot)
{}

int Goblin::take_turn(CombatCharacter& target)
{
	if (!this->is_alive())	return 0;


	if (this->_health < static_cast<int>(this->_max_health * 0.5))
	{
		this->_inventory.use_first(ItemType::HEALTH_POTION, *this);
		return 0;
	}

	return this->attack(target);
}
