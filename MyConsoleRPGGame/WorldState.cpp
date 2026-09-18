#include "WorldState.h"
#include "RandInt.h"

#include "InventoryState.h"
#include "BattleState.h"
#include "InteractState.h"
#include "MainMenuState.h"

// ===== GAME-LOOP CONTROLLER METHODS =====

void WorldState::handle_input(Game& game)
{
	char input;
	std::cin >> input;

	if (_mode == MENU)
	{
		if (input == GO_UP && _selected_option != EXPLORE)
		{
			--_selected_option;
		}
		else if (input == GO_DOWN && _selected_option != TRAVEL)
		{
			++_selected_option;
		}
		else if (input == ENTER)
		{
			switch (_selected_option)
			{
			case EXPLORE:
			{
				if (game.get_current_area()->get_hidden_items().get_size() > 0 ||
					game.get_current_area()->get_enemy_pool().get_size() > 0)
				{
					_mode = EXPLORE_EVENT;
					_explore_event = (RandInt(1, 2) == ExploreEvents::ENCOUNTER_ENEMY) ? ENCOUNTER_ENEMY : FIND_AN_ITEM;

					if (_explore_event == FIND_AN_ITEM)		_event_item = game.get_current_area()->get_random_item();
					 
					_selected_option = 0;
					break;
				}
				break;
			}

			case TALK:
				if (game.get_current_area()->get_civilians().get_size() > 0)
				{
					_mode = TALK_EVENT;
					_selected_option = 0;
					break;
				}
				break;

			case TRAVEL:
				// THERE IS ALWAYS A CONNECTED AREA
				// NO NEED FOR CHECKS
				_mode = TRAVEL_EVENT;
				_selected_option = 0;
				break;
			}
		}

		else if (input == 'i')
		{
			game.switch_state(new InventoryState());
		}

		else if (input == 'm')
		{
			game.switch_state(new MainMenuState());
		}
	}

	else if (_mode == EXPLORE_EVENT)
	{
		if (input == GO_UP && _selected_option != 0)
		{
			--_selected_option;
		}
		else if (input == GO_DOWN && (_explore_event != ENCOUNTER_ENEMY) && _selected_option != 1)
		{
			++_selected_option;
		}
		else if (input == ENTER)
		{
			switch (_selected_option)
			{
			case 0:
				switch (_explore_event)
				{
				case WorldState::ENCOUNTER_ENEMY:
					if (game.get_current_area()->choose_next_enemy() == nullptr)
					{
						_explore_event = NONE;
						_selected_option = 0;
						_mode = MENU;
						break;
					}
					game.switch_state(new BattleState(game));
					break;

				case WorldState::FIND_AN_ITEM:
					Player& player = game.get_player();

					if (!player.can_pickup_item())	break;

					player.pickup_item(_event_item);

					_event_item = nullptr;
					_selected_option = 0;
					_explore_event = NONE;
					_mode = MENU;
					break;
				}
				break;

			case 1:
				// you can't run when enemy encounters you
				// only inside battle state
				if (_explore_event == ENCOUNTER_ENEMY)	break;

				// Going back to area (not taking an item)
				_event_item = nullptr;
				_mode = MENU;
				_selected_option = 0;
				_explore_event = NONE;

				break;
			}
		}
	}
	
	else if (_mode == TALK_EVENT)
	{
		const MyDynamicArray<Civilian*>& civilians = game.get_current_area()->get_civilians();

		if (input == GO_UP && _selected_option != 0)
		{
			--_selected_option;
		}
		else if (input == GO_DOWN && _selected_option != civilians.get_size() - 1)
		{
			++_selected_option;
		}
		else if (input == ENTER)
		{
			game.switch_state(new InteractState(civilians[_selected_option]));
		}
		else if (input == 'r')
		{
			_selected_option = 0;
			_mode = MENU;
		}
	}

	else if (_mode == TRAVEL_EVENT)
	{
		const MyDynamicArray<Area*>& connected_areas = game.get_current_area()->get_connected_areas();

		if (input == GO_UP && _selected_option != 0)
		{
			--_selected_option;
		}
		else if (input == GO_DOWN && _selected_option != connected_areas.get_size() - 1)
		{
			++_selected_option;
		}
		else if (input == ENTER)
		{
			game.set_current_area(connected_areas[_selected_option]);
			_selected_option = 0;
			_mode = MENU;
		}
	}
}

void WorldState::update(Game& game)
{

}

void WorldState::render(const Game& game) const
{
	// ANSI styles
	const char* RESET = "\033[0m";
	const char* DIM = "\033[2m";
	const char* GRN = "\033[32m";
	const char* YLW = "\033[33m";
	const char* BOLD = "\033[1m";

	std::cout << "\033[3J\033[2J\033[H";

	if (!game.get_current_area())
		throw std::invalid_argument("No current area was found!");

	auto area = game.get_current_area();

	// ================= MENU MODE =================
	if (_mode == MENU)
	{
		std::cout << BOLD << "\n|===| " << area->get_name() << " |===|\n\n" << RESET;
		std::cout << "\t/ " << area->get_description() << " /\n\n";

		// ===== EXPLORE =====
		bool canExplore =
			area->get_hidden_items().get_size() > 0 ||
			area->get_enemy_pool().get_size() > 0;

		std::cout << (canExplore ? YLW : DIM);
		std::cout << (_selected_option == EXPLORE ? "> " : "  ");

		if (canExplore)
			std::cout << "Explore Area\n";
		else
			std::cout << "There is nothing to explore\n";

		std::cout << RESET;

		// ===== TALK =====
		bool canTalk = area->get_civilians().get_size() > 0;

		std::cout << (canTalk ? YLW : DIM);
		std::cout << (_selected_option == TALK ? "> " : "  ");

		if (canTalk)
			std::cout << "Talk to inhabitants\n";
		else
			std::cout << "There is no one to talk with\n";

		std::cout << RESET;

		// ===== TRAVEL =====
		std::cout << YLW << (_selected_option == TRAVEL ? "> " : "  ");
		std::cout << "Travel to another area\n" << RESET;

		// ===== ADDITIONAL OPTIONS ======

		std::cout << "\n\nFor \033[33mINVENTORY\033[0m enter [i]\n"
			<< "For \033[33mMAIN MENU\033[0m enter [M]\n";

	}

	else if (_mode == EXPLORE_EVENT)
	{
		render_explore(game);
	}

	else if (_mode == TALK_EVENT)
	{
		render_inhabitants(game);
	}

	else if (_mode == TRAVEL_EVENT)
	{
		render_travel(game);
	}
}

void WorldState::render_explore(const Game& game) const
{
	// ANSI styles
	const char* RESET = "\033[0m";
	const char* GRN = "\033[32m";
	const char* BOLD = "\033[1m";
	const char* GRAY = "\033[1;30m";

	auto area = game.get_current_area();

	std::cout << BOLD << "\n=== Exploration ===\n\n" << RESET;

	if (_explore_event == ENCOUNTER_ENEMY)
	{
		if (game.get_current_area()->choose_next_enemy() == nullptr)
		{
			std::cout << "You have been Attacked!... I'm joking there is no one left.\n\n"
				<< "Press [E] to Continue!\n";
			return;
		}
		std::cout << "\033[31m An enemy attaked You! \033[0m\n\n";
		switch (_selected_option)
		{
		case 0:
			std::cout << GRN << ">  FIGHT!\n" << RESET;
			break;
		default:
			break;
		}
	}

	else if (_explore_event == FIND_AN_ITEM)
	{
		bool can_pickup = game.get_player().can_pickup_item();

		std::cout << GRN << "You found an item!\n\n" << RESET;

		std::cout << _event_item->get_name() << '\n'
			<< "Value => " << _event_item->get_value() << " GOLD \n\n";

		switch (_selected_option)
		{
		case 0:
			std::cout << (can_pickup ? GRN : GRAY) << "> Take" << (can_pickup ? "\n" : " ( FULL INVENTORY )\n") << RESET;
			std::cout << "  Leave\n";
			break;
		case 1:
			std::cout << "  Take\n";
			std::cout << GRN << "> Leave\n" << RESET;
			break;
		default:
			break;
		}
	}
}

void WorldState::render_inhabitants(const Game& game) const
{
	// ANSI styles
	const char* RESET = "\033[0m";
	const char* GRN = "\033[32m";
	const char* YLW = "\033[33m";

	const MyDynamicArray<Civilian*>& civilians = game.get_current_area()->get_civilians();

	std::cout << "\033[3J\033[2J\033[H";


	std::cout << "\n\n======== INHABITANTS of "<< game.get_current_area()->get_name() << " =========\n\n";

	for (size_t i = 0; i < civilians.get_size(); i++)
	{
		std::cout << ((_selected_option == i) ? "\033[32m> " : "  ") << civilians[i]->get_name() << '\n' << RESET;
	}

	std::cout << "\n\nTo go back to \033[33mAREA\033[0m enter [r]\n";
}

void WorldState::render_travel(const Game& game) const
{
	// ANSI styles
	const char* RESET = "\033[0m";
	const char* GRN = "\033[32m";
	const char* YLW = "\033[33m";

	const MyDynamicArray<Area*>& connected_areas = game.get_current_area()->get_connected_areas();

	std::cout << "\033[3J\033[2J\033[H";


	std::cout << "\n\n======== INHABITANTS =========\n\n";

	for (size_t i = 0; i < connected_areas.get_size(); i++)
	{
		std::cout << ((_selected_option == i) ? "\033[32m> " : "  ") << connected_areas[i]->get_name() << '\n' << RESET;
	}
}
