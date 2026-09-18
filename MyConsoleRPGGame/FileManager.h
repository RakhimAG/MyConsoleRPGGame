#pragma once

class Game;
class MyString;

class FileManager
{
private:
    FileManager() = delete;
    ~FileManager() = delete;
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

public:

    static void save_game_data(const Game& game, const MyString path);

    static void load_game_data(Game& game, const MyString& path);
};

