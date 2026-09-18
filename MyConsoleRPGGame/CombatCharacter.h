#pragma once
#include "Entity.h"
#include "Inventory.h"

class Item;
class Armor;
class Weapon;

class CombatCharacter : public Entity
{
protected:
    int _health;
    int _max_health;

    int _base_damage;
    int _base_defense;

    static constexpr int _MAX_INVENTORY_SIZE = 20;
    Inventory _inventory;

    const Weapon* _current_weapon;

    const Armor* _current_armor;

public:

    // ------- CONSTRUCTOR ---------
    CombatCharacter(
        int id, const MyString& name,
        int health, int max_health, int damage, int defense
        );


    CombatCharacter() : Entity(),
        _max_health(10),
        _health(10),
        _base_defense(1),
        _base_damage(1),
        _inventory(_MAX_INVENTORY_SIZE),
        _current_weapon(nullptr),
        _current_armor(nullptr) {}

    CombatCharacter(const CombatCharacter& other);

    CombatCharacter(CombatCharacter&& other) = delete;
    
    // -------- OPERATORS -----------

    CombatCharacter& operator =(const CombatCharacter& other);

    CombatCharacter& operator =(CombatCharacter&& other) = delete;

    // --------- DESTRUCTOR ----------
    virtual ~CombatCharacter() override;

    // --------- GETTERS -------------

    int get_health() const;

    int get_max_health() const;

    virtual int get_damage() const;

    virtual int get_defense() const;

    const Inventory& get_inventory() const;

    Inventory& get_inventory();

    virtual void render_stats() const;

    // --------- SAVE | LOAD ----------

    void save(std::ostream& out) const override;

    void load(std::istream& in) override;

    // -------- METHODS -----------

    // === INVENTORY RELATED ===

    void equip_weapon(const Weapon* weapon);

    void unequip_weapon();

    void equip_armor(const Armor* armor);

    void unequip_armor();

    void use_item(size_t slot);

    void pickup_item(const Item* item);

    bool can_pickup_item() const;

    void remove_item(int slot);

    // === BATTLE RELATED ===
    
    // -------------------------------------------------------------
    // This methods should be used by Item classes.
    // additional_damage can be negative too for unequiping cases.
    void add_damage(int additional_damage);

    void add_defense(int additional_defense);

    // -------------------------------------------------------------
    
    virtual int take_damage(int damage);

    virtual int attack(CombatCharacter& target);

    virtual void heal(int healing_points);

    bool is_alive() const;
};