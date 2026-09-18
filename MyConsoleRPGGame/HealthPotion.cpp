#include "HealthPotion.h"
#include "CombatCharacter.h"

#include "GameExecptions.h"

HealthPotion::HealthPotion
(
	int value, MyString name,
	int healing_points
) : Item(value, name)
{
	if (healing_points > 0)		_healing_points = healing_points;
	else						_healing_points = 1;

	_item_type = ItemType::HEALTH_POTION;
}

void HealthPotion::use(CombatCharacter& user) const
{
	user.heal(_healing_points);
}

Item* HealthPotion::clone() const
{
	return new HealthPotion(*this);
}

void HealthPotion::render_item_stats() const
{
	const char* GREEN = "\033[32m";
	const char* YELLOW = "\033[33m";

	std::cout << "\n| "
		<< GREEN << "+HP   \033[0m=> "  << _healing_points<< " |\n| "
		<< YELLOW << "VALUE \033[0m=> " << _value << " |\n\n\n";
}

void HealthPotion::save(std::ostream& out) const
{
	ItemType type = ItemType::HEALTH_POTION;
	out.write(reinterpret_cast<const char*>(&type), sizeof(ItemType));

	out.write(reinterpret_cast<const char*>(&_value), sizeof(_value));

	size_t size = _name.length();
	out.write(reinterpret_cast<const char*>(&size), sizeof(size));
	out.write(_name.c_str(), size);

	out.write(reinterpret_cast<const char*>(&_healing_points), sizeof(_healing_points));
}

void HealthPotion::load(std::istream& in)
{
	in.read(reinterpret_cast<char*>(&_value), sizeof(_value));

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

	in.read(reinterpret_cast<char*>(&_healing_points), sizeof(_healing_points));
}
