#pragma once
#include "Enemy.h"

class Skeleton : public Enemy
{
public:
	Skeleton() = default;

    Skeleton(const Skeleton& other) = default;

    Skeleton& operator= (const Skeleton& other) = default;

// SKELETON BEHAIVOR :
// DIFFICULTY - EASY ( SHOULD HAVE :
// LOW HEALTH,
// LOW DAMAGE,
// LOW DEFENSE,
// NO HEAL KNOWLEDGE,
// LOW LOOT||EXP DROP)
// -> OVERALL DUMB MONSTER
    Skeleton(
        int id, const MyString& name,
        int health, int max_health, int damage, int defense,
        int exp_reward, int gold_loot
    );

	int take_turn(CombatCharacter& target) override;

    inline EnemyType get_enemy_type() const override { return EnemyType::SKELETON; }
};

