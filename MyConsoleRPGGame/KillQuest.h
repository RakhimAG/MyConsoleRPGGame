#pragma once
#include "Quest.h"
#include "Enemy.h"


class KillQuest : public Quest
{
private:
	EnemyType _target_type;
	short _needed_kills;
	short _current_kill_count;

public:
	
	KillQuest
	(
		short needed_kills, EnemyType target_type,
		int quest_id, int reward_for_completion, const MyString& title, const MyString& description
	);

	KillQuest(const KillQuest& other) = default;

	KillQuest& operator= (const KillQuest& other) = default;

	// ============= MAIN METHODS ===========

	void on_enemy_killed(const Enemy& enemy) override;

	void finish(Player& player) override;

	Quest* clone() const override;

	//=========== SAVE | LOAD ============

	void save(std::ostream& out) const override;

	void load(std::istream& in) override;

	// ========== GETTERS =============

	short get_current_kill_count() const;

	short get_required_kill_count() const;

};

