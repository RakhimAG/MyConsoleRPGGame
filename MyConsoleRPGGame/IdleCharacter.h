#pragma once
#include "Entity.h"
#include "MyString.h"
#include "MyDynamicArray.h"

class Player;

class IdleCharacter : public Entity
{
protected:
    static constexpr size_t _MAX_DIALOGUE_LINES = 7;
    MyDynamicArray<MyString> _dialogue_lines;

public:

    IdleCharacter(int id, MyString name) : Entity(id, name), _dialogue_lines(_MAX_DIALOGUE_LINES) {}

    IdleCharacter(const IdleCharacter& other) = default;

    IdleCharacter& operator= (const IdleCharacter& other) = default;

    ~IdleCharacter() = default;

    inline void add_dialogue_line(const MyString& str_line)
    {
        if (str_line.length() > 128 || _dialogue_lines.get_size() == _MAX_DIALOGUE_LINES)     return;

        _dialogue_lines.push_back(str_line);
    }

    inline const MyDynamicArray<MyString>& get_dialogue_lines() const { return _dialogue_lines; }
};
