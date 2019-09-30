#include <iostream>
#include "Menu.h"

Menu::~Menu()
{
	map<string, MenuItem *>::const_iterator iter;
	for (iter = m_menuItemList.begin(); iter != m_menuItemList.end(); iter++)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
		}
	}
	m_menuItemList.clear();
}

int Menu::addMenuItem(MenuItem *pMenuItem)
{
	int status = 0;

	if (pMenuItem == nullptr)
	{
		status = -1;
		cout << "ERROR: Menu::addMenuItem cannot add menu item value with null pointer" << endl;
	}
	else
	{
		string selector = pMenuItem->getSelector();
		map<string, MenuItem *>::iterator iter = m_menuItemList.find(selector);
		if (iter == m_menuItemList.end())
		{
			m_menuItemList.insert(pair<string, MenuItem *>(selector, pMenuItem));
		}
	}
	return status;
}

string Menu::execute()
{
	string selection = "";
	MenuItem *pMenuItem;

	while (selection != m_cmdExit)
	{
		cout << endl << this->getLabel() << endl;

		map<string, MenuItem *>::const_iterator iter;
		for (iter = m_menuItemList.begin(); iter != m_menuItemList.end(); iter++)
		{
			pMenuItem = iter->second;
			if (pMenuItem == nullptr)
			{
				cout << "ERROR: Menu::execute() menu item is null on menu " << this->getSelector() << endl;
			}
			else
			{
				cout << pMenuItem->getSelector() << " - " << pMenuItem->getLabel() << endl;
			}
		}

		cout << "Enter selection (or " << m_cmdExit << " to exit menu): ";
		cin >> selection;
		cout << endl;

		if (selection == "q" || selection == "Q")
		{
			continue;
		}
		iter = m_menuItemList.find(selection);
		if (iter == m_menuItemList.end())
		{
			cout << selection << " is not a valid menu selection.  Try again." << endl;
		}
		else
		{
			pMenuItem = iter->second;
			pMenuItem->execute();
		}
	}

	return selection;
}
