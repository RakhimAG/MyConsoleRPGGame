#pragma once
#include "Enemy.h"
class Ork : public Enemy
{
public:
	// ORK stats : (
		// HIGH HEALTH
		// AVERAGE DAMAGE
		// AVERAGE DEFENSE
		// HEALING KNOWLEDGE
		// SPECIAL ABILITY [SLAM THE GROUND AND DO EXTRA DMG]
		// HIGH LOOT||EXP DROP
	//)
	Ork(
		int id, const MyString& name,
		int health, int max_health, int damage, int defense,
		int exp_reward, int gold_loot 
	) 
	: Enemy(
		id, name, health,
		max_health, damage, defense,
		EnemyType::ORK, exp_reward, gold_loot )
	{}

	Ork(const Ork& other) = default;

	Ork& operator= (const Ork& other) = default;

	int slam_the_ground(CombatCharacter& target) const;

	int take_turn(CombatCharacter& target) override;

	inline EnemyType get_enemy_type() const override { return EnemyType::ORK; }
};

