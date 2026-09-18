#pragma once
#include "IdleCharacter.h"
#include "Quest.h"

class Player;

class Civilian : public IdleCharacter
{
protected:

    static constexpr size_t _MAX_QUESTS = 3;

    MyDynamicArray<Quest*> _quests;

public:

    Civilian(int id, MyString name);

    Civilian(const Civilian& other);

    Civilian& operator= (const Civilian& other);

    ~Civilian();
    
    const MyDynamicArray<Quest*>& get_quests() const;

    void add_quest(Quest* quest);

    void give_quest(Player& player, Quest* quest) const;

    void complete_quest(Player& player, Quest* quest) const;

    // ========== SAVE | LOAD ===========

    void virtual save(std::ostream& out) const;

    void virtual load(std::istream& in);

    // ========== GETTERS =============

    int get_quest_index(const Quest* quest) const;

    const Quest* get_quest(int index) const;

    const Quest* get_quest_by_id(int quest_id) const;

    Quest* get_quest_by_id(int quest_id);
};

