#include "Entity.h"

#include "GameExecptions.h"

// ========= CONSTRUCTORS =========
Entity::Entity(int id, MyString name) : _id(id)
{
    if (name.length() != 0)     _name = name;
    else                        _name = "UnknowEntity"; // throw ivalid arg
}

Entity::Entity(const Entity& other) : _id(other._id), _name(other._name) {}

Entity::Entity(Entity&& other) noexcept : _id(other._id)
{
    this->_name = std::move(other._name);
}

// =========== OPERATORS ==========

Entity& Entity::operator=(const Entity& other)
{
    this->_name = other._name;
    this->_id = other._id;
    return *this;
}

Entity& Entity::operator=(Entity&& other) noexcept
{
    this->_name = std::move(other._name);
    this->_id = other._id;
    return *this;
}

// ========== DESTRUCTOR =========

Entity::~Entity() = default;

// ========== GETTERS ============

const MyString& Entity::get_name() const { return _name; }

int Entity::get_id() const { return _id; }

// ========== SETTERS ============

void Entity::set_name(const MyString& new_name)
{
    if (new_name.length() == 0)     throw std::invalid_argument("No name was provided!");

    _name = new_name;
}

// ========== SAVE | LOAD ===========

void Entity::save(std::ostream& out) const
{
    out.write(reinterpret_cast<const char*>(&_id), sizeof(_id));

    size_t size = _name.length();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(_name.c_str(), size);
}

void Entity::load(std::istream& in)
{
    in.read(reinterpret_cast<char*>(&_id), sizeof(_id));

    // reading name
    size_t size = 0;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));

    if (size > MyString::get_str_capacity())
        throw FileOperationException(MyString("File is corrupted!"));

    char* raw_name = new char[size + 1];

    in.read(raw_name, size);
    raw_name[size] = '\0';

    _name = raw_name;

    delete[] raw_name;
}

