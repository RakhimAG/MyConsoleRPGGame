#include "Ork.h"
#include "RandInt.h"

int Ork::slam_the_ground(CombatCharacter& target) const
{
	return target.take_damage(static_cast<int>(target.get_health() * 0.25));
}

int Ork::take_turn(CombatCharacter& target)
{
	if (!this->is_alive())	return 0;

	short action = RandInt(1, 100);

	if (action <= 70)		return this->attack(target);
	else if (action <= 90)	this->_inventory.use_first(ItemType::HEALTH_POTION, *this);
	else					return this->slam_the_ground(target);

	return 0;
}
