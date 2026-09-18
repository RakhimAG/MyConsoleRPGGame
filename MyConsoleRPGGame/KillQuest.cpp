#include "KillQuest.h"
#include <fstream>

KillQuest::KillQuest
(
	short needed_kills, EnemyType target_type,
	int quest_id, int reward_for_completion, const MyString& title, const MyString& description
) : Quest(quest_id, reward_for_completion, title, description),
	_current_kill_count(0),
	_target_type(target_type)
{
	if (needed_kills > 0)	_needed_kills = needed_kills;
	else					_needed_kills = 1;
}

void KillQuest::on_enemy_killed(const Enemy& enemy)
{
	if (enemy.get_enemy_type() == _target_type)
	{
		_current_kill_count++;
	
		if (_current_kill_count >= _needed_kills)
		{
			_status = QuestStatus::READY_TO_TURN_IN;
		}
	}
}

void KillQuest::finish(Player& player)
{
	if (_status == QuestStatus::FINISHED)	return;

	player.add_money(_reward);

	_status = QuestStatus::FINISHED;
}

Quest* KillQuest::clone() const
{
	return new KillQuest(*this);
}

// ============= SAVE | LOAD ==============

void KillQuest::save(std::ostream& out) const
{
	out.write(reinterpret_cast<const char*>(&_quest_id), sizeof(_quest_id));
	out.write(reinterpret_cast<const char*>(&_current_kill_count), sizeof(_current_kill_count));
}

void KillQuest::load(std::istream& in)
{
	in.read(reinterpret_cast<char*>(&_quest_id), sizeof(_quest_id));
	in.read(reinterpret_cast<char*>(&_current_kill_count), sizeof(_current_kill_count));
}

// ======== GETTERS ==============

short KillQuest::get_current_kill_count() const
{
	return _current_kill_count;
}

short KillQuest::get_required_kill_count() const
{
	return _needed_kills;
}
