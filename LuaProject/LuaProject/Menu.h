#ifndef MENU_H
#define MENU_H

#include "State.h"

class Menu : public State
{
private:
public:
	Menu();
	~Menu();
	string update();
	void Render();
};
#endif