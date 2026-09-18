#pragma once

#include "CombatCharacter.h"
#include "MyDynamicArray.h"
#include "KeyItem.h"

class Game;

class Enemy;
class Quest;
class KeyItem;

class Player : public CombatCharacter
{
private:
	int _lvl;
	int _exp;

	int _gold;

	// ======== QUEST RELATED ========

	MyDynamicArray<Quest*> _active_quests;

	// ======== STATIC MEMBERS ========

	static constexpr int _MAX_QUEST_COUNT = 5;

	static int _MAX_LVL;
	static int _MAX_EXP;
	static int _MAX_GOLD;

public:
	// =========== CONSTRUCTORS =========
	Player();

	Player
	(
		int id, const MyString& name,							// ENTITY ARGS
		int health, int max_health, int defense, int damage,	// CHARACTER ARGS
		int lvl, int exp, int gold								// PLAYER ARGS
	);

	Player(const Player&) = default;

	Player(Player&&) = default;

	// =========== OPERATORS ==========
	Player& operator=(Player&&) = default;

	Player& operator=(const Player&) = default;

	// ========= DESTRUCTOR ========

	~Player() override = default;

	// ========== GETTERS ============

	int get_damage() const override;

	int get_defense() const override;

	int get_lvl() const;

	int get_exp() const;

	int get_money() const;

	void render_stats() const override;

	// =========== METHODS ===========

	void add_exp(int exp);

	void add_money(int money);

	void spend_money(int money);

	// =========== QUEST RELATED ===========

	void notify_enemy_killed(const Enemy& enemy);

	void accept_quest(Quest* quest);

	void finish_quest(Quest* quest);

	// =========== INVENTORY RELATED ===========

	void drop_item(size_t slot);

	Item* take_item_out(size_t slot);

	void remove_keyitems(KeyItemType item_type, short amount);

	// =========== SAVING/LOADING DATA ===========

	void save(std::ostream& out) const;

	void load(const Game& game, std::istream& in);

private:
	// ========== METHODS ===========

	bool is_reached_needed_exp() const;

	void level_up();

	void remove_quest(Quest* quest);
};