#ifndef MENUCOMMAND_H
#define MENUCOMMAND_H

// MenuCommand.h:

using namespace std;

#include "MenuItem.h"

class MenuCommand : public MenuItem
{
public:
	MenuCommand(string selector, string label, int(*execFunc)()) : MenuItem(selector, label) { m_execFunc = execFunc; }
	~MenuCommand(){}

	string execute(){ m_execFunc(); return ""; }

private:
	int(*m_execFunc)();	// Function to call when menu command is executed
};

#endif
