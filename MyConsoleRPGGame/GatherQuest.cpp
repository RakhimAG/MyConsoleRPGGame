#include "GatherQuest.h"

#include <fstream>

GatherQuest::GatherQuest
(
	KeyItemType target_type, short required_target_count,
	int quest_id, int reward_for_completion, const MyString& title, const MyString& description
) : Quest(quest_id, reward_for_completion, title, description),
	_target_type(target_type),
	_current_target_count(0)
{
	if (required_target_count <= 0)	_required_target_count = 1;
	else							_required_target_count = required_target_count;
}

void GatherQuest::on_item_collected(const KeyItem& item)
{
	if (item.get_key_item_type() == _target_type)
	{
		_current_target_count++;

		if (_current_target_count >= _required_target_count)
		{
			_status = QuestStatus::READY_TO_TURN_IN;
		}
	}
}

void GatherQuest::finish(Player& player)
{
	if (_status == QuestStatus::FINISHED)	return;

	player.remove_keyitems(_target_type,
		_required_target_count);

	player.add_money(_reward);

	_status = QuestStatus::FINISHED;
}

Quest* GatherQuest::clone() const
{
	return new GatherQuest(*this);
}

// ========== SAVE | LOAD ===========

void GatherQuest::save(std::ostream& out) const
{
	out.write(reinterpret_cast<const char*>(&_quest_id), sizeof(_quest_id));
	out.write(reinterpret_cast<const char*>(&_current_target_count), sizeof(_current_target_count));
}

void GatherQuest::load(std::istream& in)
{
	in.read(reinterpret_cast<char*>(&_quest_id), sizeof(_quest_id));
	in.read(reinterpret_cast<char*>(&_current_target_count), sizeof(_current_target_count));
}

// ========= GETTERS =========

KeyItemType GatherQuest::get_target_type() const
{
	return _target_type;
}

short GatherQuest::get_current_count() const
{
	return _current_target_count;
}

short GatherQuest::get_required_target_count() const
{
	return _required_target_count;
}
