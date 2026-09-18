#include "Skeleton.h"

Skeleton::Skeleton
(
	int id, const MyString& name,
	int health, int max_health, int damage, int defense,
	int exp_reward, int gold_loot
) : Enemy(
	id, name, health,
	max_health, damage, defense,
	EnemyType::SKELETON, exp_reward, gold_loot
) {}


int Skeleton::take_turn(CombatCharacter& target)
{
	if (this->is_alive() == false)
	{
		return 0;
	}

	return this->attack(target);
}
