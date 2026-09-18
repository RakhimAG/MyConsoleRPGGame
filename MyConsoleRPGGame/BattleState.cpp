#include "BattleState.h"
#include <iostream>

#include "MainMenuState.h"
#include "WorldState.h"

#include "FileManager.h"
#include "GameExecptions.h"

#include "RandInt.h"

BattleState::BattleState(Game& game) : 
	_player(&game.get_player()),
	_enemy(game.get_current_area()->choose_next_enemy()),

	_menu(BattleMenu::MAIN),
	_phase(BattlePhase::START),

	_escape_chance(50),
	_dealt_dmg(0),
	_battle_over(false)
{
}


// ========== MAIN METHODS ================

void BattleState::handle_input(Game& game)
{
	if (_battle_over)
	{
		handle_final_menu(game);
		return;
	}

	if (_phase != BattlePhase::PLAYER_TURN)		return;

	char input;
	std::cin >> input;

	switch (_menu)
	{

	case BattleMenu::MAIN:
	{
		if (input == Controls::GO_UP)
		{
			_selected_option--;

			if (_selected_option < 0)	_selected_option = _AVAILABLE_OPTIONS;
		}
		else if (input == Controls::GO_DOWN)
		{
			_selected_option++;
			if (_selected_option > _AVAILABLE_OPTIONS)	_selected_option = 0;
		}

		else if (input == Controls::ENTER)
		{
			switch (_selected_option)
			{
			case BattleMenu::ATTACK:
			{
				_dealt_dmg = _player->attack(*_enemy);
				_message = MyString("You attacked Enemy!");

				if (!_enemy->is_alive())
					_phase = BattlePhase::WON;
				else
					_phase = BattlePhase::ENEMY_TURN;

				_selected_option = 0;
				break;
			}

			case BattleMenu::ITEM:
				_selected_option = 0;
				_menu = ITEM;
				break;

			case BattleMenu::RUN:
				_phase = BattlePhase::ESCAPING;
				_message = "Trying to escape...";
				break;
			}
		}

		break;
	}

	case BattleMenu::ITEM:
	{
		if (input == GO_UP && _selected_option != 0)
		{
			--_selected_option;
		}
		else if (input == GO_DOWN && _selected_option != _player->get_inventory().get_inventory_size())
		{
			++_selected_option;
		}
		else if (input == ENTER)
		{
			if (_selected_option == _player->get_inventory().get_inventory_size())
			{
				_selected_option = 0;
				_menu = BattleMenu::MAIN;
				break;
			}
			_player->use_item(_selected_option);
			_selected_option = 0;
			_phase = BattlePhase::ENEMY_TURN;
		}
		break;
	}

	}
}


void BattleState::update(Game& game)
{
	switch (_phase)
	{
	case BattlePhase::START:
		_message = _enemy->get_name() + MyString(" APPEARS!");
		_phase = BattlePhase::PLAYER_TURN;
		break;

	case BattlePhase::PLAYER_TURN:
		// wait for input in handle_input()
		break;

	case BattlePhase::ENEMY_TURN:
	{
		int value = _enemy->take_turn(*_player);

		MyString enemy_name = _enemy->get_name();
		_dealt_dmg = value;

		if (value == 0)
		{
			_message = enemy_name + MyString(" healed itself!\n");
		}
		else
		{
			_message = enemy_name + MyString(" Attacked You!\n");
		}

		if (!_player->is_alive())
			_phase = BattlePhase::LOST;
		else
			_phase = BattlePhase::PLAYER_TURN;

		break;
	}

	case BattlePhase::ESCAPING:
	{
		int roll = RandInt(0, 100);
		char input;
		if (roll < _escape_chance)
		{
			std::cout << "\n\nYou escaped successfully!\n\n"
				<< "Prees [E] to Continue!\n";
			game.switch_state(new WorldState());
			std::cin >> input;
		}
		else
		{
			std::cout << "\n\nEscape failed!\n\n" << "Prees [E] to Continue!\n";
			std::cin >> input;
			_phase = BattlePhase::ENEMY_TURN;
		}

		break;
	}

	case BattlePhase::WON:
	{
		_battle_over = true;
		_phase = BattlePhase::LOOTING;
		_selected_option = 0;
		break;
	}

	case BattlePhase::LOST:
	{
		_battle_over = true;
		_phase = BattlePhase::LOST;
		_selected_option = 0;
		break;
	}
	}
}


void BattleState::render(const Game& game) const
{
	if (_battle_over)
	{
		render_final_menu(game);
		return;
	}

	auto draw_bar = [](int hp, int max_hp, int size)
		{
			int filled = (hp * size) / max_hp;

			std::cout << "[";
			for (int i = 0; i < size; i++)
				std::cout << (i < filled ? "#" : "-");
			std::cout << "]";
		};

	std::cout << "\033[3J\033[2J\033[H";

	std::cout << "========================\n";
	std::cout << "        BATTLE\n";
	std::cout << "========================\n\n";

	std::cout << "Player ";
	draw_bar(_player->get_health(), _player->get_max_health(), 20);
	std::cout << " " << _player->get_health() << "\n";

	std::cout << _enemy->get_name() << " ";
	draw_bar(_enemy->get_health(), _enemy->get_max_health(), 20);
	std::cout << " " << _enemy->get_health() << "\n\n";

	std::cout << _message << "\n";
	std::cout << "DMG taken : " << _dealt_dmg << "\n\n";

	if (_phase == BattlePhase::PLAYER_TURN)
	{
		switch (_menu)
		{
		case BattleState::ATTACK:

			break;
		case BattleState::ITEM:
			render_player_items();
			break;

		case BattleState::MAIN:
			std::cout << "Choose your action:\n";
			std::cout << (_selected_option == ATTACK ? "> " : "  ") << "\tAttack\n";
			std::cout << (_selected_option == ITEM ? "> " : "  ") << "\tItem\n";
			std::cout << (_selected_option == RUN ? "> " : "  ") << "\tRun\n";
			break;
		default:
			break;
		}
	}

	std::cout << "\n";
}

// ============ HELPER METHODS ============

void BattleState::handle_enemy_items()
{
	char input;
	std::cin >> input;

	if (input == GO_UP && _selected_option != 0)
	{
		--_selected_option;
	}
	else if (input == GO_DOWN && _selected_option != _enemy->get_inventory().get_inventory_size())
	{
		++_selected_option;
	}
	else if (input == ENTER)
	{
		Inventory& enemy_inventory = _enemy->get_inventory();
		if (_selected_option == enemy_inventory.get_inventory_size())
		{
			_selected_option = 0;
			_phase = BattlePhase::LOOTING;
			return;
		}
		_player->pickup_item(enemy_inventory[_selected_option]);
		enemy_inventory.destroy_item(_selected_option);
		_selected_option = 0;
		_phase = BattlePhase::LOOTING;
	}
}

// only for basic stuff.
// fancy stuff like item loot -
// handled by loot_enemy_items
void BattleState::reward_player()
{
	_player->add_exp(_enemy->get_exp_reward());
	_player->add_money(_enemy->get_gold_loot());
}

void BattleState::handle_final_menu(Game& game)
{
	if (_phase == BattlePhase::LOOTING)
	{
		char input;
		std::cin >> input;

		if (input == GO_UP)
		{
			--_selected_option;
			if (_selected_option < 0) _selected_option = 0;
		}
		else if (input == GO_DOWN)
		{
			++_selected_option;
			if (_selected_option > 1) _selected_option = 1;
		}
		else if (input == ENTER) 
		{
			switch (_selected_option)
			{
			case 0:		// continue
				reward_player();
				game.switch_state(new WorldState());
				break;
			
			case 1:
				if (!_player->can_pickup_item())
					break;

				_selected_option = 0;
				_phase = BattlePhase::LOOTING_INVENTORY;
				break;

			default:
				break;
			}
		}
	}

	else if (_phase == BattlePhase::LOOTING_INVENTORY)
	{
		handle_enemy_items();
	}

	else if (_phase == BattlePhase::LOST)
	{
		char input;
		std::cin >> input;

		if (input == ENTER) // Load last save
		{
			try {
				FileManager::load_game_data(game, MyString("save.bin"));
				game.switch_state(new WorldState());
			}
			catch (FileOperationException e)
			{
				throw FileOperationException(MyString(e.what()));
			}
		}
	}

	return;
}

void BattleState::render_final_menu(const Game& game) const
{
	std::cout << "\033[3J\033[2J\033[H";

	switch (_phase)
	{
	case BattlePhase::LOOTING:
	{
		bool player_can_loot = _player->can_pickup_item();
		const char* LOOT_ENEMY_COLOR = ( player_can_loot ? "\033[32;1m" : "\033[1;30m");
		std::cout << "|===| ENEMY LOOT |===|\n\n";

		std::cout << "EXP REWARD = " << _enemy->get_exp_reward() << '\n'
			<< "GOLD REWARD = " << _enemy->get_gold_loot() << "\n\n";

		switch (_selected_option)
		{
		case 0:
			std::cout << ">\033[32;1m  " << "Exit From Fight\n" << "\033[0m";
			std::cout << "   Loot Enemy Items\n";
			break;
		case 1:
			std::cout << "   Exit From Fight\n";
			std::cout << ">" << LOOT_ENEMY_COLOR << "  Loot Enemy Items" << (player_can_loot ? "\n" : "( INVENTORY IS FULL )\n") << "\033[0m";
			break;
		default:
			break;
		}


	}
		break;

	case BattlePhase::LOOTING_INVENTORY:
		render_enemy_items();
		break;
	case BattlePhase::LOST:

		std::cout << "|===| YOU LOST! |===|\n\n";

		std::cout << ">\033[32;1m  " << "Load Last Save\n" << "\033[0m";

		break;
	default:
		break;
	}
}

void BattleState::render_player_items() const
{
	size_t items_count = _player->get_inventory().get_inventory_size();
	const Inventory& inventory = _player->get_inventory();

	for (size_t i = 0; i < items_count; i++)
	{
		std::cout << (_selected_option == i ? ">\033[32;1m  " : "   ") << inventory[i]->get_name() << "\033[0m";
		inventory[i]->render_item_stats();
	}

	// items_count will act as a "go back" option's index
	std::cout << (_selected_option == items_count ? ">\033[32;1m  " : "   ") << "Go back\n" << "\033[0m";
}

void BattleState::render_enemy_items() const
{
	std::cout << "\033[3J\033[2J\033[H";

	std::cout << "|===| ENEMY ITEMS |===|\n\n";

	size_t items_count = _enemy->get_inventory().get_inventory_size();
	const Inventory& inventory = _enemy->get_inventory();

	for (size_t i = 0; i < items_count; i++)
	{
		std::cout << (_selected_option == i ? ">\033[32;1m  " : "   ") << inventory[i]->get_name() << "\033[0m";
		inventory[i]->render_item_stats();
	}

	// items_count will act as a "go back" option's index
	std::cout << (_selected_option == items_count ? ">\033[32;1m  " : "   ") << "Go back\n" << "\033[0m";
}
