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
	youWin = false;

	// Rooms ----
	Room* seashore = new Room("Seashore", "Lovely sun rises and touch your skin.");
	Room* fisherH = new Room("Fisherman's House", "Oldie house that smells musty and salt.");
	Room* river = new Room("River", "The sound of the running water relaxes you.");
	//Room* sea = new Room("Sea", "It seems someone is watching you under the water.");
	Room* ship = new Room("Ship - Main Deck", "It seems it was an abandoned ship.");
	Room* gunDeck = new Room("Ship - Gun Deck", "Plenty of guns and cansons are surrounded.");
	Room* orlopDeck = new Room("Ship - Orlop Deck", "It seems someone is watching you under the water.");


	Exit* seashoreToFisherH = new Exit("north", "south", "Promenade", seashore, fisherH);
	Exit* fisherHToRiver = new Exit("west", "east", "Grass Path", fisherH, river);
	//Exit* seashoreToSea = new Exit("south", "north", "Let's Swim", seashore, sea);
	Exit* seashoreToShip = new Exit("east", "west", "Wellcome on Board", seashore, ship);
	Exit* shipToGun = new Exit("down", "up", "Wellcome on Board", ship, gunDeck);
	Exit* gunToOrlop = new Exit("down", "up", "Wellcome on Board", gunDeck, orlopDeck);

	//seashoreToSea->locked = true;
	seashoreToShip->locked = true;

	entities.push_back(seashore);
	entities.push_back(fisherH);
	entities.push_back(river);


	entities.push_back(seashoreToFisherH);
	entities.push_back(fisherHToRiver);
	//entities.push_back(seashoreToSea);

	// Creatures ----
	Creature* fisherman = new Creature("Fisherman", "It's Tim, the alcoholic fisherman.", river);
	Creature* trees = new Creature("Trees", "It seems that are alive, probably.", river);
	Creature* skeleton = new Creature("Skeleton", "Doesn't look good.", orlopDeck);
	fisherman->hit_points = 50;
	trees->hit_points = 10;
	skeleton->hit_points = 30;

	entities.push_back(fisherman);
	entities.push_back(trees);
	entities.push_back(skeleton);

	// Items -----
	Item* box = new Item("Box", "Can contain something useful. It has a tiny hole...", fisherH, true);
	Item* ladder = new Item("Ladder", "Now you can reach new heighs.", trees, false);
	Item* tiny = new Item("Key", "Tiny key, may enter small holes.", fisherman, false);
	Item* potion = new Item("Potion", "Crystal glass full of a strange liquid.", skeleton, false);
	Item* rum = new Item("Rum", "A glass bottle. You can read in a ugly sticker the word 'Rum'.", seashore, false);
	box->usable = tiny;
	seashoreToShip->key = ladder;
	
	Item* axe = new Item("Axe", "A simple axe, looks sharp.", box, false, WEAPON);
	axe->min_value = 1;
	axe->max_value = 3;

	Item* rod = new Item("Rod", "A fishing rod, looks worn out.", fisherman, false, WEAPON);
	rod->min_value = 1;
	rod->max_value = 3;
	fisherman->AutoEquip();

	Item* bone = new Item("Bone", "A bone from the leg of the skeleton.", skeleton, false, WEAPON);
	bone->min_value = 4;
	bone->max_value = 7;
	skeleton->AutoEquip();

	Item* gun = new Item("Gun", "A rusty gun that still has some ammunition.", gunDeck, false, WEAPON);
	gun->min_value = 4;
	gun->max_value = 7;
	
	Item* shield = new Item("Shield", "An old wooden shield.", ship, false, ARMOUR);
	shield->min_value = 5;
	shield->max_value = 7;

	entities.push_back(ladder);
	entities.push_back(axe);
	entities.push_back(rod);
	entities.push_back(bone);
	entities.push_back(gun);
	entities.push_back(shield);
	

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
bool World::isDead()
{
	if (player->hit_points <= 0) 
	{
		return true;
	}
	return false;
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
			else if (Same(args[0], "drink"))
			{
				youWin = player->Drink(args);
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
			else if(Same(args[0], "use") || Same(args[0], "open"))
			{
				player->Use(args);
			}
			else if (Same(args[0], "give"))
			{
				player->Give(args);
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