#include "Menu.h"
#include "Windows.h"

Menu::Menu()
{
}

Menu::~Menu()
{
}

string Menu::update(float dT)
{
	if (GetKeyState('G') && GetAsyncKeyState('G'))
		return "StartGame";
	return "";
}

void Menu::Render()
{
}