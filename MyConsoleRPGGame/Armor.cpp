#include "Armor.h"
#include "CombatCharacter.h"

#include "GameExecptions.h"

Armor::Armor
(
	int value, MyString name,
	int defense_points, ArmorType armor_type
)
	: Item(value, name)
{
	if (defense_points > 0)		_defense_points = defense_points;
	else						_defense_points = 1;

	_armor_type = armor_type;
	_item_type = ItemType::ARMOR;
}

void Armor::use(CombatCharacter& user) const
{
	user.equip_armor(this);
}

void Armor::on_equip(CombatCharacter& owner) const
{
	owner.add_defense(this->_defense_points);
}

void Armor::on_unequip(CombatCharacter& owner) const
{
	owner.add_defense(-(this->_defense_points));
}

Item* Armor::clone() const
{
	return new Armor(*this);
}

void Armor::render_item_stats() const
{
	const char* CYAN = "\033[36m";
	const char* YELLOW = "\033[33m";

	std::cout << "\n| "
		<< CYAN << "+DEF   \033[0m=> "  << _defense_points << " |\n"
		<< "| " << YELLOW << "VALUE \033[0m=> " << _value << " |\n\n\n";
}

ArmorType Armor::get_armor_type() const
{
	return _armor_type;
}

void Armor::save(std::ostream& out) const
{
	ItemType type = ItemType::ARMOR;
	out.write(reinterpret_cast<const char*>(&type), sizeof(ItemType));

	out.write(reinterpret_cast<const char*>(&_value), sizeof(_value));

	size_t size = _name.length();
	out.write(reinterpret_cast<const char*>(&size), sizeof(size));
	out.write(_name.c_str(), size);

	out.write(reinterpret_cast<const char*>(&_defense_points), sizeof(_defense_points));

	out.write(reinterpret_cast<const char*>(&_armor_type), sizeof(_armor_type));
}

void Armor::load(std::istream& in)
{
	in.read(reinterpret_cast<char*>(&_value), sizeof(_value));

	// reading name
	size_t size = 0;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));

	if (size > MyString::get_str_capacity()) throw FileOperationException(MyString("File is corrupted!"));

	char* raw_name = new char[size + 1];

	in.read(raw_name, size);
	raw_name[size] = '\0';

	_name = raw_name;

	delete[] raw_name;

	// defense and type
	in.read(reinterpret_cast<char*>(&_defense_points), sizeof(_defense_points));

	in.read(reinterpret_cast<char*>(&_armor_type), sizeof(_armor_type));
}

