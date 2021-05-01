#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"

// ----------------------------------------------------
World::World()
{
	tick_timer = clock();

	// Rooms ----
	Room* seashore = new Room("Seashore", "Lovely sun rises and touch your skin.");
	Room* fisherH = new Room("Fisherman's House", "Oldie house that smells musty and salt.");
	Room* river = new Room("River", "The sound of the running water relaxes you.");
	Room* sea = new Room("Sea", "It seems someone is watching you under the water.");
	Room* boat = new Room("Boat - Main Deck", "It seems it was an abandoned boat.");
	Room* gunDeck = new Room("Boat - Gun Deck", "It seems someone is watching you under the water.");
	Room* orlopDeck = new Room("Boat - Orlop Deck", "It seems someone is watching you under the water.");


	Room* forest = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	Room* house = new Room("House", "You are inside a beautiful but small white house.");
	Room* basement = new Room("Basement", "The basement features old furniture and dim light.");



	Exit* ex1 = new Exit("west", "east", "Little path", house, forest);
	Exit* ex2 = new Exit("down", "up", "Stairs", house, basement);
	ex2->locked = true;

	Exit* seashoreToFisherH = new Exit("north", "south", "Promenade", seashore, fisherH);
	Exit* fisherHToRiver = new Exit("west", "east", "Grass Path", fisherH, river);
	Exit* seashoreToSea = new Exit("south", "north", "Let's Swim", seashore, sea);
	Exit* seashoreToBoat = new Exit("east", "west", "Wellcome on Board", seashore, boat);
	Exit* boatToGun = new Exit("down", "up", "Wellcome on Board", boat, gunDeck);
	Exit* gunToOrlop = new Exit("down", "up", "Wellcome on Board", gunDeck, orlopDeck);

	seashoreToSea->locked = true;
	seashoreToBoat->locked = true;

	entities.push_back(seashore);
	entities.push_back(fisherH);
	entities.push_back(river);

	//entities.push_back(forest);
	entities.push_back(house);
	entities.push_back(basement);

	entities.push_back(seashoreToFisherH);
	entities.push_back(fisherHToRiver);
	entities.push_back(seashoreToSea);

	// Creatures ----
	Creature* butler = new Creature("Butler", "It's James, the house Butler.", house);
	Creature* fisherman = new Creature("Fisherman", "It's James, the house Butler.", river);
	Creature* trees = new Creature("Trees", "It seems that are alive, probably.", river);
	Creature* skeleton = new Creature("Skeleton", "Doesn't has a good looking.", orlopDeck);
	fisherman->hit_points = 50;
	trees->hit_points = 10;
	skeleton->hit_points = 30;
	butler->hit_points = 10;

	entities.push_back(butler);
	entities.push_back(fisherman);
	entities.push_back(trees);

	// Items -----
	Item* mailbox = new Item("Mailbox", "Looks like it might contain something.", house);
	Item* key = new Item("Key", "Old iron key.", mailbox);
	ex2->key = key;

	Item* ladder = new Item("Ladder", "Now you can reach new heighs.", trees);
	seashoreToBoat->key = ladder;

	Item* sword = new Item("Sword", "A simple old and rusty sword.", forest, WEAPON);
	sword->min_value = 2;
	sword->max_value = 6;

	Item* axe = new Item("Axe", "A simple axe, looks sharp.", fisherH, WEAPON);
	axe->min_value = 1;
	axe->max_value = 3;

	Item* sword2(sword);
	sword2->parent = butler;

	Item* shield = new Item("Shield", "An old wooden shield.", butler, ARMOUR);
	shield->min_value = 1;
	shield->max_value = 3;
	butler->AutoEquip();

	entities.push_back(mailbox);
	entities.push_back(sword);
	entities.push_back(shield);

	entities.push_back(ladder);
	entities.push_back(axe);
	

	// Player ----
	player = new Player("Hero", "You are an awesome adventurer!", seashore);
	player->hit_points = 25;
	entities.push_back(player);
}

// ----------------------------------------------------
World::~World()
{
	for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;

	entities.clear();
}

// ----------------------------------------------------
bool World::Tick(vector<string>& args)
{
	bool ret = true;

	if(args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	GameLoop();

	return ret;
}

// ----------------------------------------------------
void World::GameLoop()
{
	clock_t now = clock();

	if((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Tick();

		tick_timer = now;
	}
}

// ----------------------------------------------------
bool World::ParseCommand(vector<string>& args)
{
	bool ret = true;

	switch(args.size())
	{
		case 1: // commands with no arguments ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "north") || Same(args[0], "n"))
			{
				(args.size() == 1) ? args.push_back("north") : args[1] = "north";
				player->Go(args);
			}
			else if(Same(args[0], "south") || Same(args[0], "s"))
			{
				(args.size() == 1) ? args.push_back("south") : args[1] = "south";
				player->Go(args);
			}
			else if(Same(args[0], "east") || Same(args[0], "e"))
			{
				(args.size() == 1) ? args.push_back("east") : args[1] = "east";
				player->Go(args);
			}
			else if(Same(args[0], "west") || Same(args[0], "w"))
			{
				(args.size() == 1) ? args.push_back("west") : args[1] = "west";
				player->Go(args);
			}
			else if(Same(args[0], "up") || Same(args[0], "u"))
			{
				(args.size() == 1) ? args.push_back("up") : args[1] = "up";
				player->Go(args);
			}
			else if(Same(args[0], "down") || Same(args[0], "d"))
			{
				(args.size() == 1) ? args.push_back("down") : args[1] = "down";
				player->Go(args);
			}
			else if(Same(args[0], "stats") || Same(args[0], "st"))
			{
				player->Stats();
			}
			else if(Same(args[0], "inventory") || Same(args[0], "i"))
			{
				player->Inventory();
			}
			else
				ret = false;
			break;
		}
		case 2: // commands with one argument ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "go"))
			{
				player->Go(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else if(Same(args[0], "equip") || Same(args[0], "eq"))
			{
				player->Equip(args);
			}
			else if(Same(args[0], "unequip") || Same(args[0], "uneq"))
			{
				player->UnEquip(args);
			}
			else if(Same(args[0], "examine") || Same(args[0], "ex"))
			{
				player->Examine(args);
			}
			else if(Same(args[0], "attack") || Same(args[0], "at"))
			{
				player->Attack(args);
			}
			else if(Same(args[0], "loot") || Same(args[0], "lt"))
			{
				player->Loot(args);
			}
			else
				ret = false;
			break;
		}
		case 3: // commands with two arguments ------------------------------
		{
			break;
		}
		case 4: // commands with three arguments ------------------------------
		{
			if(Same(args[0], "unlock") || Same(args[0], "unlk"))
			{
				player->UnLock(args);
			}
			else if(Same(args[0], "lock") || Same(args[0], "lk"))
			{
				player->Lock(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else
				ret = false;
			break;
		}
		default:
		ret =  false;
	}

	return ret;
}