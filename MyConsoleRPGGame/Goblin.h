#pragma once
#include "Enemy.h"
class Goblin : public Enemy
{
public:
// Goblin stats : (
	// AVERAGE HEALTH,
	// LOW DEFENSE,
	// AVERAGE DAMAGE,
	// RANDOM ACTIONS,
	// AVERAGE LOOT||EXP DROP
	// ) - Slightly average Enemy
	Goblin(
		int id, const MyString& name,
		int health, int max_health, int damage, int defense,
		int exp_reward, int gold_loot
	);

	Goblin(const Goblin& other) = default;

	Goblin& operator= (const Goblin& other) = default;

	int take_turn(CombatCharacter& target) override;

	inline EnemyType get_enemy_type() const override { return EnemyType::GOBLIN; }
};

