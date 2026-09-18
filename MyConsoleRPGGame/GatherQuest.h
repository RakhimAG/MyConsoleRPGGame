#pragma once
#include "Quest.h"
#include "KeyItem.h"

class GatherQuest : public Quest
{
private:
	KeyItemType _target_type;
	short _current_target_count;
	short _required_target_count;

public:

	GatherQuest
	(
		KeyItemType target_type, short required_target_count,
		int quest_id, int reward_for_completion, const MyString& title, const MyString& description
	);

	GatherQuest(const GatherQuest& other) = default;

	GatherQuest& operator= (const GatherQuest& other) = default;

	~GatherQuest() = default;

	// ========= MAIN METHODS =============

	void on_item_collected(const KeyItem& item) override;

	void finish(Player& player) override;

	Quest* clone() const override;

	// ========== SAVE | LOAD ===========

	void save(std::ostream& out) const override;

	void load(std::istream& in) override;

	// ========= GETTERS =========

	KeyItemType get_target_type() const;

	short get_current_count() const;

	short get_required_target_count() const;
};

