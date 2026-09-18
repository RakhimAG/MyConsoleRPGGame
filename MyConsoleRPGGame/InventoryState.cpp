#include "InventoryState.h"
#include "WorldState.h"

#include "Armor.h"
#include "Weapon.h"
#include "KeyItem.h"

void InventoryState::handle_input(Game& game)
{
	char input;
	std::cin >> input;

	const Inventory& inventrory = game.get_player().get_inventory();

	if (_mode == INVENTORY_DATA)
	{
		if (input == GO_UP && _selected_option != 0)
		{
			--_selected_option;
		}
		else if (input == GO_DOWN && _selected_option != inventrory.get_inventory_size() - 1)
		{
			++_selected_option;
		}
		else if (input == ENTER)
		{
			_current_item = inventrory[_selected_option];
			_item_index = _selected_option;

			_mode = ITEM_DATA;
			_selected_option = 0;
		}

		else if (input == 'i')
		{
			game.switch_state(new WorldState());
		}
	}

	else if (_mode == ITEM_DATA)
	{
		if (input == GO_UP && _selected_option != USE)
		{
			--_selected_option;
		}
		else if (input == GO_DOWN && _selected_option != GO_BACK)
		{
			++_selected_option;
		}
		else if (input == ENTER)
		{
			switch (_selected_option)
			{
			case USE:
				game.get_player().use_item(_item_index);

				_mode = INVENTORY_DATA;
				_selected_option = 0;
				break;

			case DROP:
				game.get_player().drop_item(_item_index);
				_current_item = nullptr;

				_mode = INVENTORY_DATA;
				_selected_option = 0;
				break;

			case GO_BACK:
				_mode = INVENTORY_DATA;
				_selected_option = 0;

				break;

			default:
				break;
			}
		}
	}
}

void InventoryState::update(Game& game)
{
}

void InventoryState::render(const Game& game) const
{
	const Inventory& inventory = game.get_player().get_inventory();

	std::cout << "\033[3J\033[2J\033[H";

	game.get_player().render_stats();

	if (_mode == INVENTORY_DATA)
	{
		render_inventory(inventory);
	}
	else if (_mode == ITEM_DATA)
	{
		render_item_menu();
	}
}

void InventoryState::render_inventory(const Inventory& inventory) const
{
	std::cout << "=== INVENTORY ===\n\n";

	for (size_t i = 0; i < inventory.get_inventory_size(); ++i)
	{
		const Item* item = inventory[i];

		if (i == _selected_option)
			std::cout << "> ";
		else
			std::cout << "  ";

		if (item)
			std::cout << item->get_name();
		else
			std::cout << "(empty)";

		std::cout << "\n";
	}

	std::cout << "\nUse [W/S] to navigate, [E] to select item\n"
		<< "[i] to close Inventory\n";
}

void InventoryState::render_item_menu() const
{
	std::cout << "=== ITEM MENU ===\n\n";

	std::cout << "Item: " << (_current_item ? _current_item->get_name() : MyString("Unknown")) << "\n\n";
	
	if (_current_item)	_current_item->render_item_stats();

	for (int option = USE; option <= GO_BACK; ++option)
	{
		if (_selected_option == option)
			std::cout << "> ";
		else
			std::cout << "  ";

		switch (option)
		{
		case USE:
			std::cout << "Use\n";
			break;

		case DROP:
			std::cout << "Drop\n";
			break;

		case GO_BACK:
			std::cout << "Go Back\n";
			break;

		default:
			break;
		}
	}

	std::cout << "\nUse [W/S] to navigate, [E] to confirm\n";
}
