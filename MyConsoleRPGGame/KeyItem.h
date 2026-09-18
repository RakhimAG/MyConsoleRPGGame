#pragma once
#include "Item.h"

enum class KeyItemType
{
    NONE,

    // Quest specific items (unique)
    CHEESE,
    OLD_COIN,

    // Area items (can be in quest too)
    BROKEN_PICKAXE,
    IRON_BAR,       
    MUSHROOM,
    SILVER_CHAIN,
    CRYSTAL_SHARD,

    // Enemy items (can be in quest too)
    RUSTY_BONE,
    GOBLIN_EYE,
    SKELETON_SKULL,
    ORC_TOOTH
};

class KeyItem : public Item
{
private:
    KeyItemType _key_item_type;
public:

    KeyItem() : Item(), _key_item_type(KeyItemType::NONE) {}

    KeyItem(KeyItemType type, int value, const MyString& name);

    KeyItemType get_key_item_type() const;

    // ======== MAIN METHODS ========

    void use(CombatCharacter& user) const override;

    void render_item_stats() const override;

    Item* clone() const override;

    inline ItemType get_item_type() const override { return ItemType::KEY_ITEM; }

    // ======== SAVE | LOAD =========

    void save(std::ostream& out) const override;

    void load(std::istream& in) override;
};

