#include "KeyItem.h"
#include "GameExecptions.h"

KeyItem::KeyItem(KeyItemType type, int value, const MyString& name) :
    Item(value, name),
    _key_item_type(type)
{
    _item_type = ItemType::KEY_ITEM;
}

KeyItemType KeyItem::get_key_item_type() const
{
    return _key_item_type;
}

void KeyItem::use(CombatCharacter& user) const
{
    // well just nothing, these items are for quests and nothing else
    return;
}

void KeyItem::render_item_stats() const
{
    const char* YELLOW = "\033[33m";

    std::cout << "\n| "<< YELLOW << "VALUE \033[0m=> " << _value << " |\n\n\n";
}

Item* KeyItem::clone() const
{
    return new KeyItem(*this);
}

void KeyItem::save(std::ostream& out) const
{
    ItemType type = ItemType::KEY_ITEM;
    out.write(reinterpret_cast<const char*>(&type), sizeof(ItemType));

    out.write(reinterpret_cast<const char*>(&_value), sizeof(_value));

    size_t size = _name.length();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(_name.c_str(), size);

    out.write(reinterpret_cast<const char*>(&_key_item_type), sizeof(_key_item_type));
}

void KeyItem::load(std::istream& in)
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

    in.read(reinterpret_cast<char*>(&_key_item_type), sizeof(_key_item_type));
}
