#include "CombatCharacter.h"
#include "Weapon.h"
#include "Armor.h"


// ------- CONSTRUCTORS ---------
CombatCharacter::CombatCharacter(
    int id, const MyString& name,
    int health, int max_health, int damage, int defense) : Entity(id, name), _inventory(_MAX_INVENTORY_SIZE)
{
    if (health > 0)             _health = health;
    else                        _health = 0;           

    if (max_health > 0)         _max_health = max_health;
    else                        _max_health = 0;       

    if (_max_health < _health)  _max_health = _health;

    if (damage > 0)             _base_damage = damage;
    else                        _base_damage = 1;            

    if (defense > 0)            _base_defense = defense;
    else                        _base_defense = 1;           

    _current_weapon = nullptr;
    _current_armor = nullptr;
}

CombatCharacter::CombatCharacter(const CombatCharacter& other)
    : Entity(other),
    _max_health(other._max_health),
    _health(other._health),
    _base_defense(other._base_defense),
    _base_damage(other._base_damage),
    _inventory(other._inventory),
    _current_weapon(other._current_weapon),
    _current_armor(other._current_armor)
{
}

// --------- OPERATORS -----------

CombatCharacter& CombatCharacter::operator=(const CombatCharacter& other)
{
    if (this == &other)
        return *this;

    Entity::operator=(other);

    _max_health = other._max_health;
    _health = other._health;
    _base_defense = other._base_defense;
    _base_damage = other._base_damage;

    _inventory = other._inventory;

    _current_weapon = other._current_weapon;
    _current_armor = other._current_armor;

    return *this;
}

// --------- DESTRUCTOR -----------

CombatCharacter::~CombatCharacter()
{
    // Inventory will handle the cleaning
    _current_weapon = nullptr;
    _current_armor = nullptr;
}

// --------- GETTERS -------------

int CombatCharacter::get_health() const       { return _health; }

int CombatCharacter::get_max_health() const   { return _max_health; }

int CombatCharacter::get_damage() const       { return _base_damage; }

int CombatCharacter::get_defense() const      { return _base_defense; }

const Inventory& CombatCharacter::get_inventory() const
{
    return _inventory;
}

Inventory& CombatCharacter::get_inventory()
{
    return _inventory;
}

void CombatCharacter::render_stats() const
{
    std::cout << "\n=== EQUIPMENT ===\n";

    if (_current_weapon)
    {
        std::cout << "\033[31mWEAPON: \033[0m"
            << _current_weapon->get_name()
            << " (+" << _current_weapon->get_damage_points() << " \033[31mATK"
            << "\033[0m) \n";
    }
    else
    {
        std::cout << "\033[90mWEAPON: None\033[0m\n";
    }

    if (_current_armor)
    {
        std::cout << "\033[36mARMOR:  \033[0m"
            <<  _current_armor->get_name() << 
            " (+" << _current_armor->get_defense_points() << " \033[36mDEF"
            << "\033[0m) \n";
    }
    else
    {
        std::cout << "\033[90mARMOR: None\033[0m\n";
    }

    std::cout << "\033[32mHP:  \033[0m" << _health;
    std::cout <<  '/' << _max_health << "\033[0m\n";

    std::cout << "\033[31mATK: \033[0m" << _base_damage << "\033[0m\n";
    std::cout << "\033[36mDEF: \033[0m" << _base_defense << "\033[0m\n";
}

// ----------- SAVE | LOAD --------------

void CombatCharacter::save(std::ostream& out) const 
{
    Entity::save(out);

    out.write(reinterpret_cast<const char*>(&_health), sizeof(_health));
    out.write(reinterpret_cast<const char*>(&_max_health), sizeof(_max_health));

    out.write(reinterpret_cast<const char*>(&_base_damage), sizeof(_base_damage));
    out.write(reinterpret_cast<const char*>(&_base_defense), sizeof(_base_defense));

    _inventory.save_data(out);

    //----------------------------------------------------------------------
    // storing indicies for saving and loading issues.
    // because pointers can't handle that if they are just references,
    // and not the actual owners of their data.
    //----------------------------------------------------------------------

    int current_weapon_index = _inventory.get_item_index(_current_weapon);
    int current_armor_index = _inventory.get_item_index(_current_armor);

    out.write(reinterpret_cast<const char*>(&current_weapon_index), sizeof(current_weapon_index));
    out.write(reinterpret_cast<const char*>(&current_weapon_index), sizeof(current_armor_index));

}

void CombatCharacter::load(std::istream& in)
{
    Entity::load(in);

    in.read(reinterpret_cast<char*>(&_health), sizeof(_health));
    in.read(reinterpret_cast<char*>(&_max_health), sizeof(_max_health));

    in.read(reinterpret_cast<char*>(&_base_damage), sizeof(_base_damage));
    in.read(reinterpret_cast<char*>(&_base_defense), sizeof(_base_defense));

    _inventory.load_data(in);

    int current_weapon_index = -1;
    int current_armor_index = -1;

    in.read(reinterpret_cast<char*>(&current_weapon_index), sizeof(current_weapon_index));
    in.read(reinterpret_cast<char*>(&current_weapon_index), sizeof(current_armor_index));

    if (current_weapon_index != -1)
        _current_weapon = dynamic_cast<const Weapon*>(_inventory.get_item(current_weapon_index));
    else
        _current_weapon = nullptr;

    if (current_armor_index != -1)
        _current_armor = dynamic_cast<const Armor*>(_inventory.get_item(current_armor_index));
    else
        _current_armor = nullptr;
}

// -------- INVENTORY RELATED -----------

void CombatCharacter::equip_weapon(const Weapon* weapon)
{
    if (!weapon)    throw std::invalid_argument("Weapon was null pointer");
    if (weapon == _current_weapon)    return;

    if (_inventory.get_item_index(weapon) == -1)
    {
        // for cheaters maybe? lol
        throw std::invalid_argument("Equipping a Weapon, that does not exist in inventory!");
    }

    unequip_weapon();

    _current_weapon = weapon;
    weapon->on_equip(*this);
}

void CombatCharacter::unequip_weapon()
{
    if (_current_weapon)
    {
        _current_weapon->on_unequip(*this);
        _current_weapon = nullptr;
    }
}

void CombatCharacter::equip_armor(const Armor* armor)
{
    if (!armor)    throw std::invalid_argument("Armor does not exist\n");
    if (armor == _current_armor)    return;


    if (_inventory.get_item_index(armor) == -1)
    {
        throw std::invalid_argument("Equipping an Armor, that does not exist in inventory!");
    }

    unequip_armor();
    
    _current_armor = armor;
    armor->on_equip(*this);
}

void CombatCharacter::unequip_armor()
{
    if (_current_armor)
    {
        _current_armor->on_unequip(*this);
        _current_armor = nullptr;
    }
}

void CombatCharacter::use_item(size_t slot)
{
    if (slot >= _inventory.get_inventory_size())	throw std::invalid_argument("Player::use_item(slot) - slot => is out of range");

    _inventory.use_item(slot, *this);
}

void CombatCharacter::pickup_item(const Item* item)
{
    if (!item)
        return;
    else if (_inventory.get_inventory_size() == _MAX_INVENTORY_SIZE)
        return;

    _inventory.add_item(item);
}

bool CombatCharacter::can_pickup_item() const
{
    if (_inventory.get_inventory_size() == _MAX_INVENTORY_SIZE)
        return false;

    return true;
}

void CombatCharacter::remove_item(int slot)
{
    _inventory.destroy_item(slot);
} 

// ---------- METHODS --------------

void CombatCharacter::add_damage(int additional_damage)
{
    this->_base_damage += additional_damage;
}

void CombatCharacter::add_defense(int additional_defense)
{
    this->_base_damage += additional_defense;
}

// Will be controlled by BattleState
int CombatCharacter::take_damage(int damage)
{
    damage -= _base_defense;

    if (damage <= 0)    damage = 1;

    (_health - damage <= 0) ? _health = 0 : _health -= damage;

    return damage;
}

bool CombatCharacter::is_alive() const
{
    if (_health > 0)
        return true;

    return false;
}

// Will be controlled by BattleState
void CombatCharacter::heal(int healing_points)
{
    _health += healing_points;

    if (_health > _max_health)  _health = _max_health;
}

// Will be controlled by BattleState
int CombatCharacter::attack(CombatCharacter& target)
{
    return target.take_damage(this->get_damage());
}