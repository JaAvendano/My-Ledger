#ifndef MENU_H
#define MENU_H

// Menu.hpp:

using namespace std;

#include <string>
#include <map>
#include "MenuItem.h"

class Menu : public MenuItem {
public:
	Menu(string selector, string label, string cmdExit) : MenuItem(selector, label) { m_cmdExit = cmdExit; }
	~Menu();

	int addMenuItem(MenuItem *menuItem);

	// Override of the virtual function from MenuItem, this displays the selector
	// and label for each menu item in the menu and prompts the user to select an
	// item to be executed.
	string execute();

private:
	string m_cmdExit;		// Selector to exit this menu
	map<string, MenuItem *> m_menuItemList;

	int accData;
};

#endif
