#pragma once
#include "CombatCharacter.h"
#include "EnemyTypes.h"

class Item;

class Enemy : public CombatCharacter
{
protected:
    EnemyType _enemy_type;
    int _exp_reward;
    int _gold_loot;

public:

    // ======== CONSTRUCTOR =========

    Enemy
    (
        int id, const MyString& name,
        int health, int max_health, int damage, int defense,
        EnemyType enemy_type, int exp_reward, int gold_loot
    );

    Enemy() = default;

    Enemy(const Enemy& other) = default;

    Enemy(Enemy&& other) = default;

    // ======== OPERATORS =======
    Enemy& operator= (const Enemy& other) = default;

    Enemy& operator= (Enemy&& other) = default;

    // ======== DESTRUCTOR ==========
    ~Enemy() override = default;

    // ========= GETTERS ============

    int get_exp_reward() const;

    int get_gold_loot() const;

    virtual EnemyType get_enemy_type() const { return EnemyType::NONE; }

    // ========== METHOD ==========

    // Every class that will inherit from Enemy,
    // will have its own behaivor depending on :
    // own health, items.
    int virtual take_turn(CombatCharacter& targer) = 0;

    // ========= SAVE | LOAD ==========

    virtual void save(std::ostream& out) const;

    virtual void load(std::istream& in);

};

