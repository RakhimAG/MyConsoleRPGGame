#pragma once
#include "MyString.h"

class Entity
{
protected:
    MyString _name;
    int _id;

public:
    // ========= CONSTRUCTORS =========
    Entity() : _id(-1), _name() {}

    Entity(int id, MyString name);

    Entity(const Entity& other);

    Entity(Entity&& other) noexcept;

    // =========== OPERATORS ==========

    Entity& operator=(const Entity& other);

    Entity& operator=(Entity&& other) noexcept;

    // ========== DESTRUCTOR =========

    virtual ~Entity();

    // ============ GETTERS ==============

    const MyString& get_name() const;

    int get_id() const;

    // =========== SETTERS ===========

    void set_name(const MyString& new_name);

    // =========== SAVE | LOAD ==========

    virtual void save(std::ostream& out) const;

    virtual void load(std::istream& in);
};