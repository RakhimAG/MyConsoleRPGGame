#include "Game.h"
#include <cstdlib>	// FOR RANDOMNESS IN Enemy.RandInt();

// GAME_STATES
#include "MainMenuState.h"

// ITEMS
#include "Armor.h"
#include "Weapon.h"
#include "HealthPotion.h"
#include "KeyItem.h"

// QUESTS
#include "KillQuest.h"
#include "GatherQuest.h"

// ENTITY : ENEMY
#include "Skeleton.h"
#include "Goblin.h"
#include "Ork.h"

// ENTITY : CIVILIAN
#include "Civilian.h"
#include "TraderCivilian.h"

// EXECPTIONS
#include "GameExecptions.h"

size_t Game::_STARTER_AREA_INDEX = VILLAGE;

Game::Game() : 
    _locations(_MAX_LOCATIONS),
    _current_area(nullptr),
    _is_running(false),
    _state_change_requested(false),
    _current_state(nullptr),
    _next_state(nullptr)
{
	srand(time(NULL));
}

void Game::run()
{
    init();

	_is_running = true;

	_current_state = new MainMenuState();

	while (_is_running)
	{
		//=========================================
		// First should run the render before handle_input
		// handle_input will stop the loop until user's input is given (if there is one)
		// that way render will happen again after user choosed an option -
		// and not render when nothing changes.
		//=========================================
        _current_state->update(*this);
		_current_state->render(*this);
		_current_state->handle_input(*this);


        apply_state_change();
	}
}

Game::~Game()
{
    delete _current_state;
    _current_state = nullptr;

    for (size_t i = 0; i < _locations.get_size(); i++)
    {
        delete _locations[i];
    }

    _locations.clear();
}

void Game::set_current_area(Area* other_area)
{
    if (!other_area)    return;

    _current_area = other_area;
}

void Game::switch_state(GameState* new_state)
{
    if (_next_state)        delete _next_state;

    _next_state = new_state;
    _state_change_requested = true;
}

void Game::quit()
{
	_is_running = false;
}

Player& Game::get_player()
{
    return _player;
}

const Player& Game::get_player() const
{
    return _player;
}

MyDynamicArray<Area*>& Game::get_locations()
{
    return _locations;
}

const MyDynamicArray<Area*>& Game::get_locations() const
{
    return _locations; 
}

const Area* Game::get_area(size_t index) const
{
    if (index > _locations.get_size())      return nullptr;

    return _locations[index];
}

Area* Game::get_area(size_t index)
{
    if (index > _locations.get_size())      return nullptr;

    return _locations[index];
}

int Game::get_area_index(Area* area) const
{
    for (int i = 0; i < _locations.get_size(); i++)
    {
        if (area == _locations[i])      return i;
    }
    
    return -1;
}

const Area* Game::get_current_area() const
{
    return _current_area;
}

Area* Game::get_current_area()
{
    return _current_area;
}

const Area* Game::get_starting_area() const
{
    return _locations[_STARTER_AREA_INDEX];
}

Area* Game::get_starting_area()
{
    return _locations[_STARTER_AREA_INDEX];
}

Quest* Game::find_quest_by_id(int id)
{
    
    return nullptr;
}

void Game::apply_state_change()
{
    if (!_state_change_requested)   return;

    delete _current_state;
    _current_state = _next_state;

    _next_state = nullptr;
    _state_change_requested = false;
}

// ============ FOR FILE MANAGER =============

void Game::save(std::ostream& out) const
{
    //========================================================================
    // Saving locations first - because player's data
    // depend on it. Exactly i'm talking about _active_quests.
    // Player does not own them, Civilians do.
    // So when loading civilians will reconstruct their quests -
    // Then player should find them through quest_id and assign references.
    //========================================================================
    _locations.save(out);

    _player.save(out);

    int current_area_index = Game::get_area_index(_current_area);
    out.write(reinterpret_cast<const char*>(&current_area_index), sizeof(current_area_index));
}

void Game::load(std::istream& in)
{
    // locations, their size is fixed - so no assigning is needed
    size_t size = 0;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));

    if (size != _locations.get_size()) throw FileOperationException(MyString("Locations load failed due to corrupted size!"));

    for (size_t i = 0; i < size; i++)
    {
        _locations[i]->load(in);
    }

    // player
    _player.load(*this, in);

    // last area player was
    int index;
    in.read(reinterpret_cast<char*>(&index), sizeof(index));
    _current_area = Game::get_area(index);
}

// ============ METHODS INITIALIZING OBJECTS =============
void Game::init()
{
    InitializePlayer();

    InitializeAreas();
    ConnectAreas();

    InitializeHiddenItems();

    InitializeCivilians();

    InitializeEnemies();
}

void Game::InitializePlayer()
{
    _player = Player
    (
        1,                  // id,
        MyString("Hero"),   // name,

        20,                 // health,
        20,                 // max_health,
        3,                  // defense,
        3,                  // damage,

        1,                  // lvl
        0,                  // exp
        20                  // gold
    );

    // -------- STARTER ITEMS -------------
    HealthPotion* health_pot = new HealthPotion
    (
        7,                              // value
        MyString("Small HP potion"),
        5                               // healing points
    );

    Weapon* wooden_sword = new Weapon
    (
        5,                              // value
        MyString("Wooden Sword"),
        5,                              // damage
        WeaponType::SWORD
    );

    Armor* leather_boots = new Armor
    (
        1,                              // value
        MyString("Leather Boots"),
        2,                              // defense points
        ArmorType::BOOTS
    );

    _player.pickup_item(health_pot);

    _player.pickup_item(wooden_sword);
    _player.use_item(1);

    _player.pickup_item(leather_boots);
    _player.use_item(2);

    delete health_pot;
    delete wooden_sword;
    delete leather_boots;
}

void Game::InitializeAreas()
{
    // ---------- AREAS ----------

    // --------------------------------------
    // Village will be main Area 
    // where player will appear first time
    // --------------------------------------
    Area* village = new Area(
        MyString("Village"),
        MyString("A quiet village.. it feels safe in here")
    );

    Area* forest = new Area(
        MyString("Dark Forest"),
        MyString("Tall trees block most of the sunlight. Strange noises echo around.")
    );

    Area* cave = new Area(
        MyString("Forgotten Cave"),
        MyString("A cold damp cave filled with bones and old mining tools.")
    );

    Area* camp = new Area(
        MyString("Ork Camp"),
        MyString("A rough camp with large tents, bonfires, and weapon racks.")
    );

    _locations.push_back(village);
    _locations.push_back(forest);
    _locations.push_back(cave);
    _locations.push_back(camp);
}

void Game::InitializeCivilians()
{
// Village civilians

    Civilian* bob_civilian = new Civilian(2, MyString("Bob"));
    bob_civilian->add_dialogue_line(MyString("Hello I'm Bob, the Bob!"));
    bob_civilian->add_dialogue_line(MyString("My friends call me Bobster, really strange."));
    bob_civilian->add_dialogue_line(MyString("Hero? What a strange name for a child.."));

    bob_civilian->add_quest
    (
        new KillQuest(
            1,                      // required kills
            EnemyType::SKELETON,    
            101,                    // quest id
            20,                     // reward
            MyString("Bonely"),
            MyString("Kill a Skeleton, after completion tell to Bob!")
        )
    );

    _locations[VILLAGE]->add_civilian(bob_civilian);
    

    // =======      BIBA        ======
    Civilian* biba_civilian = new Civilian(3, MyString("Biba"));

    biba_civilian->add_dialogue_line(MyString("I hate Bob."));
    biba_civilian->add_dialogue_line(MyString("Hey, check out my brother Boba, he trades good stuff!"));
    biba_civilian->add_dialogue_line(MyString("Why are we standing there like a statue?"));
    biba_civilian->add_dialogue_line(MyString("Sometimes I dream about cheese.. a lot of em"));

    biba_civilian->add_quest
    (
        new GatherQuest
        (
            KeyItemType::CHEESE,
            1,                          // required amount
            102,                        // quest id
            50,                         // reward
            MyString("Biba's Cheese"),
            MyString("Find cheese and bring it to Biba!")
        )
    );

    _locations[VILLAGE]->add_civilian(biba_civilian);


    // ========         BOBA        =============

    TraderCivilian* boba_trader = new TraderCivilian(4, MyString("Boba"));

    boba_trader->add_dialogue_line(MyString("Heey, did my brotha tell ya to check out my shop? GOOD!"));
    boba_trader->add_dialogue_line(MyString("If you here to bazar with me.. then wrong place buddy."));
    boba_trader->add_dialogue_line(MyString("This shop is legaly constructed! TRUST ME!"));
    boba_trader->add_dialogue_line(MyString("If you wanna work for my shop, then you will do it for free."));

    boba_trader->add_quest
    (
        new KillQuest
        (
            3,                          // required kills
            EnemyType::GOBLIN,
            103,                        // quest id
            150,                        // reward
            MyString("Rob the Robbers"),
            MyString("Kill 3 Goblins for Boba. They love hiding in Dark Forst.")
        )
    );

    boba_trader->add_quest
    (
        new GatherQuest
        (
            KeyItemType::OLD_COIN,
            1,                          // required amount
            104,                        // quest id
            70,                         // reward
            MyString("Legendary Coin"),
            MyString("Boba lost old coin and wants it back to his collection! Find it.")
        )
    );


    // ----------- WEAPONS AND ARMOR TO TRADE ------------
    Item* iron_sword = new Weapon
    (
        50,                     // value
        MyString("Iron Sword"),
        12,                      // damage
        WeaponType::SWORD
    );

    Item* bloody_axe = new Weapon
    (
        120,                     // value
        MyString("Bloody Axe"),
        28,                      // damage
        WeaponType::GREAT_AXE
    );

    boba_trader->add_item(iron_sword);
    delete iron_sword;

    boba_trader->add_item(bloody_axe);
    delete bloody_axe;

    Item* cool_helmet = new Armor
    (
        25,                         // value
        MyString("Cool Helmet"),
        10,                         // defense points
        ArmorType::HELMET
    );

    Item* diamond_chestplate = new Armor
    (
        300,                         // value
        MyString("Diamond Chestplate"),
        30,                         // defense points
        ArmorType::CHESTPLATE
    );

    boba_trader->add_item(cool_helmet);
    delete cool_helmet;

    boba_trader->add_item(diamond_chestplate);
    delete diamond_chestplate;

    // --------- HEALTH POTIONS --------- 
    constexpr short small_pots_amount = 3;
    constexpr short aver_pots_amount = 5;
    constexpr short big_pots_amount = 3;

    Item* small_pot = new HealthPotion
    (
        10,
        MyString("Small HP potion"),
        5
    );

    Item* aver_pot = new HealthPotion
    (
        20,
        MyString("HP potion"),
        15
    );

    Item* big_burger = new HealthPotion
    (
        50,
        MyString("Big Burger"),
        30
    );

    for (short i = 0; i < small_pots_amount; i++)
    {
        boba_trader->add_item(small_pot);
    }
    delete small_pot;

    for (short i = 0; i < aver_pots_amount; i++)
    {
        boba_trader->add_item(aver_pot);
    }
    delete aver_pot;

    for (short i = 0; i < big_pots_amount; i++)
    {
        boba_trader->add_item(big_burger);
    }
    delete big_burger;

    _locations[VILLAGE]->add_civilian(boba_trader);
}

void Game::InitializeEnemies()
{
    // ======= FOREST ENEMIES ========

    Enemy* forest_goblin = new Goblin
    (
        5,                          // id 
        MyString("Forest Goblin"),
        30,                         // health
        30,                         // max health
        2,                          // damage
        2,                          // defense
        18,                         // exp reward
        10                          // gold reward
    );

    HealthPotion* small_health_pot = new HealthPotion
    (
        10,                             // value
        MyString("Small HP potion"),
        5                               // hp points
    );

    forest_goblin->pickup_item(small_health_pot);
    forest_goblin->pickup_item(small_health_pot);
    delete small_health_pot;

    Armor* underpants = new Armor
    (
        15,
        MyString("Underpants"),
        7,
        ArmorType::CHESTPLATE
    );

    Weapon* dagger = new Weapon
    (
        20,                         // value
        MyString("Dagger"),
        6,                          // dmg
        WeaponType::SWORD
    );

    forest_goblin->pickup_item(dagger);
    forest_goblin->use_item(2);
    delete dagger;

    forest_goblin->pickup_item(underpants);
    forest_goblin->use_item(3);
    delete underpants;

    // items loot

    KeyItem* goblin_eye = new KeyItem(KeyItemType::GOBLIN_EYE, 5, MyString("Goblin Eye"));

    forest_goblin->pickup_item(goblin_eye);
    delete goblin_eye;

    _locations[FOREST]->add_enemy(forest_goblin);

    Enemy* forest_skeleton = new Skeleton
    (
        6,                          // id
        MyString("Lost Skeleton"),
        10,                         // current health
        10,                         // max health
        3,                          // damage
        1,                          // defense
        12,                         // exp reward
        6                           // gold reward
    );

    Weapon* bone_sword = new Weapon
    (
        5,                         // value
        MyString("Bone Sword"),
        3,                         // dmg
        WeaponType::SWORD
    );

    forest_skeleton->pickup_item(bone_sword);
    forest_skeleton->use_item(0);
    delete bone_sword;

    // items loot

    KeyItem* skeleton_skull = new KeyItem(KeyItemType::SKELETON_SKULL, 5, MyString("Skeleton Skull"));

    forest_skeleton->pickup_item(skeleton_skull);

    _locations[FOREST]->add_enemy(forest_skeleton);

    // Cave enemies

    Enemy* ancient_skeleton = new Skeleton
    (
        7,                              // id
        MyString("Ancient Skeleton"),
        17,                             // health
        17,                             // max health
        3,                              // damage
        1,                              // defense
        35,                             // gold reward
        18                              // exp reward
    );

    Weapon* nordic_axe = new Weapon
    (
        15,
        MyString("Nordic Axe"),
        7,
        WeaponType::GREAT_AXE
    );

    Armor* nordic_helmet = new Armor
    (
        10,
        MyString("Nordic Helmet"),
        5,
        ArmorType::HELMET
    );

    KeyItem* rusty_bone = new KeyItem(KeyItemType::RUSTY_BONE, 5, MyString("Rusty Bone"));

    ancient_skeleton->pickup_item(rusty_bone);
    delete rusty_bone;

    ancient_skeleton->pickup_item(nordic_axe);
    ancient_skeleton->use_item(1);
    delete nordic_axe;

    ancient_skeleton->pickup_item(nordic_helmet);
    ancient_skeleton->use_item(2);
    delete nordic_helmet;

    _locations[CAVE]->add_enemy(ancient_skeleton);

    Enemy* warrior_skeleton = new Skeleton
    (
        8,                              // id
        MyString("Skeleton Warrior"),
        12,                             // health
        12,                             // max health
        3,                              // dmg
        2,                              // def
        17,                             // gold reward
        12                              // exp reward
    );

    Weapon* iron_sword = new Weapon
    (
        15,                     // value
        MyString("Rusty Iron Sword"),
        7,                      // damage
        WeaponType::SWORD
    );

    warrior_skeleton->pickup_item(iron_sword);
    warrior_skeleton->use_item(0);
    delete iron_sword;

    warrior_skeleton->pickup_item(skeleton_skull);
    delete skeleton_skull;

    _locations[CAVE]->add_enemy(warrior_skeleton);

    // Camp enemies

    Enemy* ork_grunt = new Ork
    (
        9,                      // id
        MyString("Ork Grunt"),
        70,                     // health
        70,                     // max health
        10,                     // dmg
        7,                      // def
        70,                     // exp reward
        40                      // gold reward
    );

    Weapon* big_club = new Weapon
    (
        40,
        MyString("Big Club"),
        20,
        WeaponType::CLUB
    );

    HealthPotion* aver_health_potion = new HealthPotion
    (
        20,                         // value
        MyString("HP potion"),
        15                          // hp points
    );

    KeyItem* ork_tooth = new KeyItem
    (
        KeyItemType::ORC_TOOTH,
        80,
        MyString("Orc Tooth")
    );

    ork_grunt->pickup_item(big_club);
    ork_grunt->use_item(0);
    delete big_club;

    ork_grunt->pickup_item(aver_health_potion);
    ork_grunt->pickup_item(ork_tooth);

    _locations[CAMP]->add_enemy(ork_grunt);


    Enemy* ork_brute = new Ork
    (
        10,                      // id
        MyString("Ork Brute"),
        120,                     // health
        120,                     // max health
        15,                     // dmg
        10,                      // def
        160,                     // exp reward
        100                      // gold reward
    );

    Weapon* greater_axe = new Weapon
    (
        100,
        MyString("Greater Axe"),
        20,
        WeaponType::GREAT_AXE
    );

    Armor* bone_boots = new Armor
    (
        30,
        MyString("Bone Boots"),
        5,
        ArmorType::BOOTS
    );

    ork_brute->pickup_item(bone_boots);
    ork_brute->use_item(0);
    delete bone_boots;

    ork_brute->pickup_item(greater_axe);
    ork_brute->use_item(1);
    delete greater_axe;

    ork_brute->pickup_item(ork_tooth);
    ork_brute->pickup_item(aver_health_potion);
    delete aver_health_potion;
    delete ork_tooth;

    _locations[CAMP]->add_enemy(ork_brute);
}

void Game::ConnectAreas()
{
// =========================
// CONNECT AREAS
// =========================

    _locations[VILLAGE]->connect_area(_locations[FOREST]);

    _locations[FOREST]->connect_area(_locations[VILLAGE]);
    _locations[FOREST]->connect_area(_locations[CAVE]);
    _locations[FOREST]->connect_area(_locations[CAMP]);

    _locations[CAVE]->connect_area(_locations[FOREST]);

    _locations[CAMP]->connect_area(_locations[FOREST]);
}

void Game::InitializeHiddenItems()
{
    // -------- FOREST ---------
    _locations[FOREST]->add_hidden_item
    (
        new KeyItem
        (
            KeyItemType::CHEESE,
            5,
            MyString("Cheese")
        )
    );

    _locations[FOREST]->add_hidden_item
    (
        new KeyItem
        (
            KeyItemType::MUSHROOM,
            5,
            MyString("Mushroom")
        )
    );

    _locations[FOREST]->add_hidden_item
    (
        new HealthPotion
        (
            5,
            MyString("Healthy Water"),
            5
        )
    );

    // -------- CAVE ---------

    _locations[CAVE]->add_hidden_item
    (
        new KeyItem
        (
            KeyItemType::IRON_BAR,
            20,
            MyString("Iron Bar")
        )
    );

    _locations[CAVE]->add_hidden_item
    (
        new KeyItem
        (
            KeyItemType::BROKEN_PICKAXE,
            10,
            MyString("Broken Pickaxe")
        )
    );

    _locations[CAVE]->add_hidden_item
    (
        new KeyItem
        (
            KeyItemType::OLD_COIN,
            3,
            MyString("Old Coin")
        )
    );

    _locations[CAVE]->add_hidden_item
    (
        new KeyItem
        (
            KeyItemType::CRYSTAL_SHARD,
            50,
            MyString("Crystal Shard")
        )
    );

    _locations[CAVE]->add_hidden_item
    (
        new HealthPotion
        (
            20,
            MyString("Old Wine"),
            15
        )
     );

    // camp

    _locations[CAMP]->add_hidden_item
    (
        new KeyItem
        (
            KeyItemType::SILVER_CHAIN,
            100,
            MyString("Silver Chain")
        )
    );

    _locations[CAMP]->add_hidden_item
    (
        new HealthPotion
        (
            50,
            MyString("Large HP potion"),
            30
        )
    );
}
