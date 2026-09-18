#pragma once
#include "GameState.h"
#include "MyString.h"

class BattleState : public GameState
{
    MyString _message;

    Enemy* const _enemy;
    Player* const _player;

    enum class BattlePhase
    {
        START,
        PLAYER_TURN,
        ENEMY_TURN,
        WON,
        LOOTING,
        LOOTING_INVENTORY,
        LOST,
        ESCAPING,
    };

    enum BattleMenu
    {
        ATTACK,
        ITEM,
        RUN,
        MAIN
    };

    static constexpr short _AVAILABLE_OPTIONS = 3;

    BattleMenu _menu;

    BattlePhase _phase;

    //-------------------------------------
    // this is like a percent
    // it means 40 = 40% or 100 = 100%
    //-------------------------------------
    int _escape_chance;

    int _dealt_dmg;

    bool _battle_over;

public:

    BattleState(Game& game);

	void handle_input(Game& game) override;

	void update(Game& game) override;

	void render(const Game& game) const override;

private:

    void render_enemy_items() const;

    void handle_enemy_items();

    void reward_player();

    void handle_final_menu(Game& game);

    void render_final_menu(const Game& game) const;

    void render_player_items() const;
};

