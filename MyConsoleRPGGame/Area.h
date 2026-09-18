#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Civilian.h"
#include "Item.h"

// CONTAINERS
#include "MyDynamicArray.h"
#include "MyString.h"

class Area
{
private:
    // ======== STATIC MEMBERS =========
    static constexpr size_t _MAX_CIVILIANS_CAPACITY = 5;
    static constexpr size_t _MAX_ENEMIES_CAPACITY = 7;
    static constexpr size_t _MAX_ITEMS_CAPACITY = 5;
    static constexpr size_t _MAX_CONNECTABLE_AREAS = 3;

    MyString _name;

    MyString _description;

    // ===========================================================
    // Containers below should store pointers instead of values.
    // By help of the pointers it will be much better
    // to reference these objects in quests, battle and etc.
    // Prevents unnecessary copying objects.
    // ===========================================================

    MyDynamicArray<Enemy*> _enemy_pool;

    MyDynamicArray<Civilian*> _civilians;

    MyDynamicArray<Item*> _collectable_items;

    MyDynamicArray<Area*> _connected_areas;

public:

    Area(const MyString& name,
        const MyString& description);

    Area(const Area&) = delete;

    Area& operator=(const Area&) = delete;

    ~Area();

    void add_enemy(Enemy* enemy);

    void add_civilian(Civilian* civilian);

    void add_hidden_item(Item* item);

    void connect_area(Area* area);

    // ========= SAVING | LOADING ============

    void save(std::ostream& out) const;

    void load(std::istream& in);

    // ============ GETTERS ================

    const MyDynamicArray<Civilian*>& get_civilians() const;

    const MyDynamicArray<Item*>& get_hidden_items() const;

    const MyDynamicArray<Enemy*>& get_enemy_pool() const;

    const MyDynamicArray<Area*>& get_connected_areas() const;

    const Enemy* choose_next_enemy() const;

    Enemy* choose_next_enemy();

    const Item* get_random_item() const;

    const MyString& get_description() const;

    const MyString& get_name() const;
};

