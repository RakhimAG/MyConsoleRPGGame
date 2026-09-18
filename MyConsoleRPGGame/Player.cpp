#include "Player.h"
#include "Enemy.h"
#include "Quest.h"

#include "GameExecptions.h"
#include "Game.h"

#include "Armor.h"
#include "Weapon.h"

// ========= STATIC MEMBERS =========

int Player::_MAX_LVL = 20;

int Player::_MAX_EXP = 2000;

int Player::_MAX_GOLD = 25000;

// ========== CONSTRUCTROS ===========
Player::Player() : CombatCharacter(), _lvl(1), _exp(0), _gold(0) {}

Player::Player
(
	int id, const MyString& name,							// ENTITY ARGS
	int health, int max_health, int defense, int damage,	// CHARACTER ARGS
	int lvl, int exp, int gold								// PLAYER ARGS
)
	: CombatCharacter(id, name, health, max_health, damage, defense),
	_lvl(lvl), _exp(exp), _gold(gold), _active_quests(_MAX_QUEST_COUNT)
{
	if (lvl > 0)		_lvl = lvl;
	else				throw std::invalid_argument("LVL can't be zero or below!");

	if (exp >= 0)		_exp = exp;
	else				throw std::invalid_argument("EXP given as negative number!");

	if (gold >= 0)		_gold = gold;
	else				throw std::invalid_argument("GOLD given as negative number!");
}

// ========== GETTERS ==========

int Player::get_damage() const 
{
	return _base_damage;
}

int Player::get_defense() const
{
	return _base_defense;
}

int Player::get_money() const { return _gold; }

void Player::render_stats() const
{
	CombatCharacter::render_stats();

	std::cout << "\033[35mLVL:  \033[0m" << _lvl << '\n';
	std::cout << "\033[34mEXP:  \033[0m" << _exp << "\n";
	std::cout << "\033[33mGOLD: \033[0m" << _gold << '\n';
}

int Player::get_lvl() const { return _lvl; }

int Player::get_exp() const { return _exp; }

// ======== METHODS ==========

void Player::add_exp(int exp)
{
	if (exp < 0)	throw std::invalid_argument("Tried to add negative EXP!");

	_exp += exp;

	if (is_reached_needed_exp())	level_up();
	// -----------------------------------------------------------
	// this statement will prevent player ( that reached max 20 LVL )
	// to gain more exp more than _MAX_EXP.
	// ------------------------------------------------------------
	if (exp > _MAX_EXP)				_exp = _MAX_EXP;
}

// ------------------------------------
// formula for each lvl's needed exp:
// needed_exp = lvl * 10
// ------------------------------------
bool Player::is_reached_needed_exp() const
{
	if (_exp >= _lvl * 10 and _lvl < _MAX_LVL)	
		return true;

	return false;
}

void Player::level_up()
{
	_exp -= _lvl * 10;
	_lvl++;
	_max_health += _lvl * 2;
	_base_damage += _lvl * 2;
	_base_defense += _lvl * 2;
}

void Player::add_money(int money)
{
	if (_gold + money > _MAX_GOLD)	_gold = _MAX_GOLD;
	else						_gold += money;
}

void Player::spend_money(int money)
{
	if (_gold -= money <= 0)
		return; // throw not_enough_money CUSTOM

	_gold -= money;
}


// =========== QUEST RELATED ===========

void Player::notify_enemy_killed(const Enemy& enemy)
{
	for (size_t i = 0; i < _active_quests.get_size(); i++)
	{
		_active_quests[i]->on_enemy_killed(enemy);
	}
}

void Player::accept_quest(Quest* quest)
{
	if (quest)
	{
		_active_quests.push_back(quest);
		quest->set_status(QuestStatus::IN_PROGRESS);
	}
}

// ---------------------------------------
// This method will be called when player
// reaches back to Entity that gave this quest.
// ------------------------------------------
void Player::finish_quest(Quest* quest)
{
	if (quest)
	{
		quest->set_status(QuestStatus::FINISHED);

		remove_quest(quest);
	}
}

void Player::remove_quest(Quest* quest)
{
	if (!quest)
		return;

	for (size_t i = 0; i < _active_quests.get_size(); i++)
	{
		if (_active_quests[i] == quest)
		{
			_active_quests.remove_at(i);
			return;
		}
	}
}

// ============ INVENTORY RELATED ==============

void Player::remove_keyitems(KeyItemType item_type, short amount)
{
	
	if (amount <= 0)
		return;

	for (size_t i = 0; i < _inventory.get_inventory_size() && amount > 0;)
	{
		KeyItem* item = dynamic_cast<KeyItem*>(_inventory[i]);

		if (!item)
		{
			i++;
			continue;
		}

		if (item->get_key_item_type() == item_type)
		{
			delete _inventory[i];

			_inventory.destroy_item(i);

			amount--;
		}
		else
		{
			i++;
		}
	}
	
}

void Player::drop_item(size_t slot)
{
	if (slot > _inventory.get_inventory_size())		return;

	if (static_cast<const Item*>(_current_armor) == _inventory[slot])
	{
		_current_armor = nullptr;
	}
	else if (static_cast<const Item*>(_current_weapon) == _inventory[slot])
	{
		_current_weapon = nullptr;
	}

	_inventory.destroy_item(slot);
}

Item* Player::take_item_out(size_t slot)
{
	return _inventory.take_item_out(slot);
}


// ========== SAVE | LOAD =================

void Player::save(std::ostream& out) const
{
	CombatCharacter::save(out);
	
	// simple stats of player lvl, exp, gold
	out.write(reinterpret_cast<const char*>(&_lvl), sizeof(_lvl));
	out.write(reinterpret_cast<const char*>(&_exp), sizeof(_exp));

	out.write(reinterpret_cast<const char*>(&_gold), sizeof(_gold));

	// active_quests
	size_t quests_count = _active_quests.get_size();

	out.write(reinterpret_cast<const char*>(&quests_count), sizeof(quests_count));

	for (size_t i = 0; i < quests_count; i++)
	{
		int quest_id = _active_quests[i]->get_quest_id();
		out.write(reinterpret_cast<const char*>(&quest_id), sizeof(quest_id));
	}
}

void Player::load(const Game& game, std::istream& in)
{
	CombatCharacter::load(in);

	// simple stats of player lvl, exp, gold
	in.read(reinterpret_cast<char*>(&_lvl), sizeof(_lvl));
	in.read(reinterpret_cast<char*>(&_exp), sizeof(_exp));

	in.read(reinterpret_cast<char*>(&_gold), sizeof(_gold));

	// active_quests
	// ============================================================
	// Because this part depends on Civilians, and player
	// does not know about them, I will open up a way through game
	// ============================================================
	size_t quests_count = 0;

	in.read(reinterpret_cast<char*>(&quests_count), sizeof(quests_count));
	
	_active_quests.clear();

	for (size_t i = 0; i < quests_count; i++)
	{
		int quest_id = -1;
		in.read(reinterpret_cast<char*>(&quest_id), sizeof(quest_id));

		const MyDynamicArray<Civilian*>& civilians = game.get_area(0)->get_civilians();

		for (size_t i = 0; i < civilians.get_size(); i++)
		{
			Quest* activated_quest = civilians[i]->get_quest_by_id(quest_id);
			
			if (activated_quest)
			{
				_active_quests.push_back(activated_quest);
			}
		}
	}

	if (_active_quests.get_size() != quests_count)
		throw FileOperationException(MyString("Count does not match the count of _active_quests saved _ids"));
}