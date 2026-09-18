#include "FileManager.h"
#include "MyString.h"

#include <fstream>
#include "GameExecptions.h"

#include "Game.h"
#include "MainMenuState.h"


void FileManager::save_game_data(const Game& game, const MyString path)
{
    std::ofstream file;

    try
    {
        file.open(path.c_str(), std::ios::binary | std::ios::trunc);

        if (!file.is_open())
        {
            throw FileOperationException(MyString("Cannot open file: ") + MyString(path));
        }

        game.save(file);

        if (file.fail())
        {
            throw FileOperationException(MyString("Saving failed into file: ") + MyString(path));
        }

        file.close();
    }
    catch (FileOperationException e)
    {
        if (file.is_open()) file.close();
        throw FileOperationException( MyString("Saving could not happen : ") + MyString(e.what()) );
    }
}

void FileManager::load_game_data(Game& game, const MyString& path)
{
    std::ifstream file;

    try
    {
        file.open(path.c_str(), std::ios::binary);

        if (!file.is_open())
        {
            throw FileOperationException(MyString("Cannot open file: ") + MyString(path));
        }

        game.load(file);

        if (file.fail())
        {
            throw FileOperationException(MyString("Loading failed from file: ") + MyString(path));
        }

        file.close();
    }
    catch (FileOperationException e)
    {
        if (file.is_open()) file.close();
        game.set_current_area(game.get_starting_area());
        game.switch_state(new MainMenuState());
    }
}
