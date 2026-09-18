#pragma once
#include <iostream>
#include "MyString.h"
#include "Player.h"

enum class QuestStatus
{
    INACTIVE,
    IN_PROGRESS,
    READY_TO_TURN_IN,
    FINISHED
};

class Player;
class Enemy;
class KeyItem;

class Quest
{
protected:
    QuestStatus _status;
    int _quest_id;
    int _reward;

    MyString _title;
    MyString _description;

public:

    Quest(int quest_id, int reward_for_completion, const MyString& title,
        const MyString& description)
        :
        _quest_id(quest_id),
        _reward(reward_for_completion),
        _title(title),
        _description(description),
        _status(QuestStatus::INACTIVE)
    {
    }

    Quest(const Quest& other) = default;

    Quest& operator= (const Quest& other) = default;

    virtual ~Quest() = default;
    
    virtual void on_enemy_killed(const Enemy&) {}

    virtual void on_item_collected(const KeyItem&) {}

    virtual void finish(Player& player) = 0;

    virtual Quest* clone() const = 0;

    virtual void save(std::ostream& out) const = 0;

    virtual void load(std::istream& in) = 0;

    inline QuestStatus get_status() const
    {
        return _status;
    }

    // This state means that quest is completed and awaits
    // for player to talk to the Entity that gave him this quest.
    inline bool is_completed() const
    {
        return _status == QuestStatus::READY_TO_TURN_IN;
    }

    inline bool is_finished() const
    {
        return _status == QuestStatus::FINISHED;
    }

    inline const MyString& get_description() const
    {
        return _description;
    }

    inline const MyString& get_title() const
    {
        return _title;
    }

    inline int get_quest_id() const
    {
        return _quest_id;
    }

    inline int get_reward() const
    {
        return _reward;
    }

    inline void set_status(QuestStatus new_status)
    {
        _status = new_status;
    }
};
