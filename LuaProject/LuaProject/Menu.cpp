#include "Menu.h"
#include "Windows.h"

Menu::Menu()
{
}

Menu::~Menu()
{
}

string Menu::update()
{
	if (GetKeyState('G') && GetAsyncKeyState('G'))
		return "StartGame";
	return "";
}

void Menu::Render()
{
}