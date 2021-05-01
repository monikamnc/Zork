#include <iostream>
#include "room.h"
#include "exit.h"
#include "globals.h"
#include "item.h"

// ----------------------------------------------------
Item::Item(const char* title, const char* description, Entity* parent, bool isClosed, ItemType item_type) :
Entity(title, description, parent), usable(NULL), item_type(item_type)
{
	type = ITEM;
	min_value = max_value = 0;
	closed = isClosed;
}

// ----------------------------------------------------
Item::~Item()
{}

// ----------------------------------------------------
void Item::Look() const
{
	cout << "\n" << name << "\n";
	cout << description << "\n";

	list<Entity*> stuff;
	FindAll(ITEM, stuff);

	if(stuff.size() > 0)
	{
		cout << "It contains: " << "\n";
		for (list<Entity*>::const_iterator it = stuff.begin(); it != stuff.cend(); ++it)
		{
			if (this->closed)
			{
				cout << "There's something but you can't look in it.\n";
			}
			else
			{
				cout << (*it)->name << "\n";
			}
		}
	}
}

// ----------------------------------------------------
int Item::GetValue() const
{
	return Roll(min_value, max_value);
}