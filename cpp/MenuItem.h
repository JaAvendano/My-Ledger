#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>



using namespace std;

class MenuItem {
public:
	MenuItem(string selector, string label){ m_selector = selector; m_label = label; }
	virtual ~MenuItem(){}

	string getLabel(){ return m_label; }
	string getSelector(){ return m_selector; }
	virtual string execute() = 0;

private:
	string m_label;		// Text to display in menu for this item
	string m_selector;	// Text which user must enter to select this menu item
};

#endif
