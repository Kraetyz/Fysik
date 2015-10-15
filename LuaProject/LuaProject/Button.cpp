#include "Button.h"

Button::Button() : GameObject()
{
}

Button::Button(vec2 pos, vec2 size, string name) : GameObject(pos, name, size.x, size.y, "rectangle")
{
	this->name = name;
}

string Button::getName()
{
	return name;
}