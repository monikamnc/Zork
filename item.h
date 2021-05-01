#ifndef __Item__
#define __Item__

#include <string>
#include "entity.h"

class Room;

using namespace std;

enum ItemType
{
	COMMON,
	WEAPON,
	ARMOUR
};

class Item : public Entity
{
public:
	Item(const char* name, const char* description, Entity* parent, bool isClosed, ItemType item_type = COMMON);
	~Item();

	void Look() const;
	int GetValue() const;

public :
	int min_value;
	int max_value;
	bool closed;
	Entity* usable;
	ItemType item_type;

};

#endif //__Item__