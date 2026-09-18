#include "InteractState.h"
#include "WorldState.h"

#include "RandInt.h"

InteractState::InteractState(Civilian* civilian) : _civilian(civilian)
{
	if (!civilian)		throw std::invalid_argument("Civilian was passed as nullptr into InteractState!");
}

// ========== MAIN METHODS ================

void InteractState::handle_input(Game& game)
{
	char input;
	std::cin >> input;
	if (input == ENTER)
	{
		game.switch_state(new WorldState());
	}
}

void InteractState::update(Game& game)
{
}

void InteractState::render(const Game& game) const
{
	std::cout << "\033[3J\033[2J\033[H";

	std::cout << "|===| " << _civilian->get_name() << " |===|\n\n";

	const MyDynamicArray<MyString>& dialogue_lines = _civilian->get_dialogue_lines();

	int roll = RandInt(0, _civilian->get_dialogue_lines().get_size() - 1);

	std::cout << "/ " << dialogue_lines[roll] << " /\n\n";

	std::cout << "This state was not finished in time :\\ \n";
	std::cout << "Feel free to enter [E] to go back, Or enter anything to see other dialogue lines lol\n\n";
}
