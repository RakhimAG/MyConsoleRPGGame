#include "Weapon.h"
#include "CombatCharacter.h"

Weapon::Weapon
(
	int value, MyString name,
	int weapon_damage, WeaponType weapon_type
) : Item(value, name)
{
	if (weapon_damage > 0)	_weapon_damage = weapon_damage;
	else					_weapon_damage = 1;

	_weapon_type = weapon_type;
	_item_type = ItemType::WEAPON;
}

void Weapon::use(CombatCharacter& user) const
{
	user.equip_weapon(this);
}

void Weapon::on_equip(CombatCharacter& owner) const
{
	owner.add_damage(_weapon_damage);
}

void Weapon::on_unequip(CombatCharacter& owner) const
{
	owner.add_damage(- _weapon_damage);
}

Item* Weapon::clone() const
{
	return new Weapon(*this);
}

void Weapon::render_item_stats() const
{
	const char* RED = "\033[31m";
	const char* YELLOW = "\033[33m";

	std::cout << "\n| "
		<< RED << "+DMG   \033[0m=> " << _weapon_damage << " |\n| "
		<< YELLOW << "VALUE \033[0m=> " << _value << " |\n\n\n";
}

WeaponType Weapon::get_weapon_type() const
{
	return _weapon_type;
}

void Weapon::save(std::ostream& out) const
{
	ItemType type = ItemType::WEAPON;
	out.write(reinterpret_cast<const char*>(&type), sizeof(ItemType));

	out.write(reinterpret_cast<const char*>(&_value), sizeof(_value));

	size_t size = _name.length();
	out.write(reinterpret_cast<const char*>(&size), sizeof(size));
	out.write(_name.c_str(), size);

	out.write(reinterpret_cast<const char*>(&_weapon_damage), sizeof(_weapon_damage));
	out.write(reinterpret_cast<const char*>(&_weapon_type), sizeof(_weapon_type));
}

void Weapon::load(std::istream& in)
{
	in.read(reinterpret_cast<char*>(&_value), sizeof(_value));

	// reading name
	size_t size = 0;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));

	//if (size > MyString::get_str_capacity())
		//throw FileOperationException(MyString("File is corrupted!"));

	char* raw_name = new char[size + 1];

	in.read(raw_name, size);
	raw_name[size] = '\0';

	_name = raw_name;

	delete[] raw_name;

	// weapon dmg and type
	in.read(reinterpret_cast<char*>(&_weapon_damage), sizeof(_weapon_damage));
	in.read(reinterpret_cast<char*>(&_weapon_type), sizeof(_weapon_type));

}

