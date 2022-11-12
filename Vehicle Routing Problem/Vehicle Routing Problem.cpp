#include <iostream>

#include "menu.h"

int main()
{
	//TODO replace srand(time(NULL)) with a better rng generator?
	srand(time(NULL)); // Begin pseudo random number generation.
	menu menu;
	menu.loadSettings();
}
