#include "MainMenuState.h"
#include "WorldState.h"

#include "FileManager.h"
#include "GameExecptions.h"

enum Options
{
    NEW_GAME,
    SAVE_GAME,
    LOAD_GAME,
    EXIT,
};

void MainMenuState::handle_input(Game& game)
{

	char input;
	std::cin >> input;

	if (input == GO_UP && _selected_option != NEW_GAME)
	{
		--_selected_option;
	}
	else if (input == GO_DOWN && _selected_option != EXIT)
	{
		++_selected_option;
	}
	else if (input == ENTER)
	{
		switch (_selected_option)
		{
		case NEW_GAME:
            if (game.get_current_area() == nullptr)
            {
                game.set_current_area(game.get_starting_area());
            }

            game.switch_state(new WorldState());
			break;
        
        case SAVE_GAME:
        {
            try
            {
                FileManager::save_game_data(game, MyString("save.bin"));
            }
            catch (FileOperationException e)
            {
                throw e;
            }
            break;
        }
        case LOAD_GAME:
        {
            try
            {
                FileManager::load_game_data(game, MyString("save.bin"));
                game.switch_state(new WorldState());
            }
            catch (FileOperationException e)
            {
                throw e;
            }
            break;
        }

        case EXIT:
			game.quit();
			break;
		}
	}
}

void MainMenuState::update(Game& game) { return; }

void MainMenuState::render(const Game& game) const
{
    const char* RESET = "\033[0m";

    // Clear screen + move cursor home
    std::cout << "\033[3J\033[2J\033[H";

    // ===== TITLE =====
    std::cout << "\033[33;1m"; // bright yellow

    std::cout << "\tMy Console RPG Game!!!\n\n\n";

    // ===== MENU =====
    std::cout << "\033[0m"; // reset

    switch (_selected_option)
    {
    case NEW_GAME:
        std::cout << (game.get_current_area() ? ">\033[32;1m  Continue Current Game\n" : ">\033[32;1m  New Game\n") << RESET;

        std::cout << "\033[37m   Save Game\n" << RESET;     // gray

        std::cout << "\033[37m   Load Game\n" << RESET;     // gray

        std::cout << "\033[37m   Exit\n" << RESET;           // gray
        break;

    case SAVE_GAME:
        std::cout << (game.get_current_area() ? "\033[37m   Continue Current Game\n" : "\033[37m   New Game\n") << RESET;

        std::cout << "> \033[33;1m Save Game\n" << RESET;   // yellow

        std::cout << "\033[37m   Load Game\n"<< RESET;     // gray

        std::cout << "\033[37m   Exit\n" << RESET;           // gray
        break;

    case LOAD_GAME:
        std::cout << (game.get_current_area() ? "\033[37m   Continue Current Game\n" : "\033[37m   New Game\n") << RESET;

        std::cout << "\033[37m   Save Game\n" << RESET;     // gray

        std::cout << "> \033[33;1m Load Game\n" << RESET;   // yellow

        std::cout << "\033[37m   Exit\n" << RESET;           // gray
        break;

    case EXIT:
        std::cout << (game.get_current_area() ? "\033[37m   Continue Current Game\n" : "\033[37m   New Game\n") << RESET;

        std::cout << "\033[37m   Save Game\n" << RESET;     // gray

        std::cout << "\033[37m   Load Game\n" << RESET;     // gray

        std::cout << ">\033[31;1m  Exit\n" << RESET;  // red
        break;
    }

    // ===== FOOTER =====
    std::cout << "\033[0m";

    std::cout << "\n";
    std::cout << "\033[36m";
    std::cout << "------------------------------\n";
    std::cout << "\033[0m";

    std::cout << "\nControls: [W/S] = move, [E] = select\n";
}
