#include "Enemy.h"
#include "Item.h"

// ======== CONSTRUCTOR =========

Enemy::Enemy
(
    int id, const MyString& name,
    int health, int max_health, int damage, int defense,
    EnemyType enemy_type, int exp_reward, int gold_loot
)
    : CombatCharacter(id, name, health, max_health, damage, defense), 
    _enemy_type(enemy_type)
{
    if (exp_reward >= 0)     _exp_reward = exp_reward;
    else                     throw std::invalid_argument("EXP reward of Enemy can't be negative!");

    if (gold_loot >= 0)      _gold_loot = gold_loot;
    else                     throw std::invalid_argument("GOLD loot of Enemy can't be negative!");
}

// ========= GETTERS ============

int Enemy::get_exp_reward() const { return _exp_reward; }

int Enemy::get_gold_loot() const { return _gold_loot; }

// ======== SAVE | LOAD ==========

void Enemy::save(std::ostream& out) const
{
    CombatCharacter::save(out);

    out.write(reinterpret_cast<const char*>(&_enemy_type), sizeof(_enemy_type));
    out.write(reinterpret_cast<const char*>(&_exp_reward), sizeof(_exp_reward));
    out.write(reinterpret_cast<const char*>(&_gold_loot), sizeof(_gold_loot));
}

void Enemy::load(std::istream& in)
{
    CombatCharacter::load(in);

    in.read(reinterpret_cast<char*>(&_enemy_type), sizeof(_enemy_type));
    in.read(reinterpret_cast<char*>(&_exp_reward), sizeof(_exp_reward));
    in.read(reinterpret_cast<char*>(&_gold_loot), sizeof(_gold_loot));
}