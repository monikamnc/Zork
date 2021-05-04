#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"

using namespace std;

#define BACKSPACE "\033[D\033[K"

// -------------------------------------------------
int main()
{
	char key;
	string player_input;
	vector<string> args;
	args.reserve(10);

	cout << "Welcome to MoniZork!\n";
	cout << "You have to find and drink the eternal life potion to escape from this universe.\n";
	cout << "--------------------------------------------------------------------------------\n";

	World my_world;

	args.push_back("look");

	while(1)
	{
		if(_kbhit() != 0)
		{
			key = _getch();
			if(key == '\b') // backspace
			{
				if(player_input.length() > 0)
				{
					player_input.pop_back();
					//cout << BACKSPACE;
					cout << '\b';
					cout << " ";
					cout << '\b';
				}
			}
			else if(key != '\r') // return
			{
				player_input += key;
				cout << key;
			}
			else
				Tokenize(player_input, args);
		}

		if(args.size() > 0 && Same(args[0], "quit"))
			break;

		if(my_world.Tick(args) == false)
			cout << "\nSorry, I do not understand your command.\n";

		if (my_world.isDead()) {
			break;
		}
		if (my_world.youWin)
		{
			break;
		}

		if(args.size() > 0)
		{
			args.clear();
			player_input = "";
			cout << "> ";
		}

	}

	cout << "\nThanks for playing, See ya!\n";
	system("pause");
	return 0;
}