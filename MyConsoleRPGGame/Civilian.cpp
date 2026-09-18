#include "Civilian.h"
#include "Player.h"

#include "RandInt.h"

#include <fstream>
#include "GameExecptions.h"

Civilian::Civilian(int id, MyString name) : IdleCharacter(id, name), _quests(_MAX_QUESTS) {}

Civilian::Civilian(const Civilian& other) : IdleCharacter(other)
{
	for (size_t i = 0; i < other._quests.get_size(); i++)
	{
		_quests.push_back(other._quests[i]->clone());
	}
}


Civilian& Civilian::operator=(const Civilian& other)
{
	if (this == &other)		return *this;

	IdleCharacter::operator=(other);

	for (size_t i = 0; i < _quests.get_size(); i++)
	{
		delete _quests[i];
	}

	_quests.clear();

	for (size_t i = 0; i < other._quests.get_size(); i++)
	{
		_quests.push_back(other._quests[i]->clone());
	}

	return *this;
}

Civilian::~Civilian()
{
	for (size_t i = 0; i < _quests.get_size(); i++)
	{
		delete _quests[i];
	}
}

const MyDynamicArray<Quest*>& Civilian::get_quests() const
{
	return _quests;
}

void Civilian::add_quest(Quest* quest)
{
	if (!quest)		return;

	_quests.push_back(quest);
}

// -------------------------------------------
// GameState child will be responsible		/
// for choosing which quest to give.	   /
// ----------------------------------------
void Civilian::give_quest(Player& player, Quest* quest) const
{
	if (!quest)		return;

	if (quest->get_status() == QuestStatus::INACTIVE)	player.accept_quest(quest);
}

void Civilian::complete_quest(Player& player, Quest* quest) const
{
	if (!quest)		return;

	quest->finish(player);
}

// ============ SAVE | LOAD ===========

void Civilian::save(std::ostream& out) const
{
	_quests.save(out);
}

void Civilian::load(std::istream& in)
{
	size_t size = 0;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));

	if (size != _quests.get_size())
	{
		throw FileOperationException(MyString("Civilian load failed due to corrupted _quests size! Civilain name : ") + this->get_name());
	}


	for (size_t i = 0; i < size; i++)
	{
		_quests[i]->load(in);
	}
}

// ============= GETTERS ==============

int Civilian::get_quest_index(const Quest* quest) const
{
	for (int i = 0; i < _quests.get_size(); i++)
	{
		if (quest == _quests[i])	return i;
	}

	return -1;
}

const Quest* Civilian::get_quest(int index) const
{
	if (index < 0 || index >= _quests.get_size())	return nullptr;

	return _quests[index];
}

const Quest* Civilian::get_quest_by_id(int quest_id) const
{
	for (size_t i = 0; i < _quests.get_size(); i++)
	{
		if (quest_id == _quests[i]->get_quest_id())		return _quests[i];
	}

	return nullptr;
}

Quest* Civilian::get_quest_by_id(int quest_id)
{
	for (size_t i = 0; i < _quests.get_size(); i++)
	{
		if (quest_id == _quests[i]->get_quest_id())		return _quests[i];
	}

	return nullptr;
}

// BELOW ARE THE PART THAT SHOULD BE DONE BY INTRECACTION-STATE

// !!!! FOR IdleNPCInteractionState 
//void Civilian::interact(Player& player)
//{
//    bool interacting = true;
//
//    while (interacting)
//    {
//        system("cls");
//
//        std::cout << "Talking to "
//            << _name << "\n\n";
//
//        std::cout << "1. Talk\n";
//        std::cout << "2. Quest\n";
//        std::cout << "3. Exit\n";
//
//        int option;
//        std::cin >> option;
//
//        switch (option)
//        {
//        case 1:
//            talk();
//            break;
//
//        case 2:
//            handle_quests(player);
//            break;
//
//        case 3:
//            interacting = false;
//            break;
//        }
//    }
//}

//void Civilian::handle_quests(Player& player)
//{
//    for (size_t i = 0; i < _quests.get_size(); i++)
//    {
//        Quest* quest = _quests[i];
//
//        switch (quest->get_status())
//        {
//        case QuestStatus::INACTIVE:
//            std::cout << "Would you like to take this quest :\n"
//                << quest->get_title() << '\n'
//                << quest->get_description() << '\n';
//            break;
//
//        case QuestStatus::READY_TO_TURN_IN:
//            quest->finish(player);
//
//            std::cout << "Thanks for completing "
//                << quest->get_title() << '\n';
//
//            std::cout << "Here is your "
//                << quest->get_reward()
//                << " gold!\n";
//
//            std::cout << "\nPress enter to continue...";
//
//            std::cin.ignore();
//            std::cin.get();
//
//            break;
//        }
//    }
//}
