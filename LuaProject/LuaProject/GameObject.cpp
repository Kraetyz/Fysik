#include "GameObject.h"

void GameObject::setPos(vec2 pos)
{
	corners[NW] = vec2(pos.x - 0.01, pos.y + 0.01);
	corners[NE] = vec2(pos.x + 0.01, pos.y + 0.01);
	corners[SW] = vec2(pos.x - 0.01, pos.y - 0.01);
	corners[SE] = vec2(pos.x + 0.01, pos.y - 0.01);
}

void GameObject::setColor(vec3 col)
{
	color = col;
}

GameObject::GameObject()
{
	color = vec3(1, 1, 1);
}


GameObject::GameObject(vec2 pos, string clr, float sizeX, float sizeY)
{
	float ratio = 1280.0f / 768.0f;
	origPos = pos;
	pos.x = pos.x*0.064;
	pos.y = pos.y*0.064*ratio;
	corners[NW] = vec2(pos.x - sizeX*0.032, pos.y - sizeY*0.032*ratio);
	corners[NE] = vec2(pos.x + sizeX*0.032, pos.y - sizeY*0.032*ratio);
	corners[SW] = vec2(pos.x - sizeX*0.032, pos.y + sizeY*0.032*ratio);
	corners[SE] = vec2(pos.x + sizeX*0.032, pos.y + sizeY*0.032*ratio);
	if (clr == "green")
	{
		color = vec3(0, 1, 0);
	}
	if (clr == "blue")
	{
		color = vec3(0, 0, 1);
	}
	if (clr == "yellow")
	{
		color = vec3(1, 1, 0);
	}

	if (clr == "red")
	{
		color = vec3(1, 0, 0);
	}
	this->clr = clr;
	this->rForward = 1;
	this->gForward = 1;
	this->bForward = 1;
}

void GameObject::getCorners(vec2 toFill[])
{
	toFill[NW] = corners[NW];
	toFill[NE] = corners[NE];
	toFill[SW] = corners[SW];
	toFill[SE] = corners[SE];
}

vec3 GameObject::getColor()
{
	return color;
}

string GameObject::getColorString()
{
	return clr;
}

vec2 GameObject::getOrigPos()
{
	return origPos;
}

void GameObject::moveX(float newX, int index)
{
	corners[index].x = newX;
}

void GameObject::moveY(float newY, int index)
{
	corners[index].y = newY;
}

void GameObject::updateColor()
{
	this->color = vec3(this->color.b + (0.015*this->bForward) , this->color.g + (0.02*this->gForward) , this->color.r + (0.03*this->rForward));

	if (this->color.b > 0.99)
	{
		this->bForward = -1;
	}

	if (this->color.g >0.99)
	{
		this->gForward = -1;
	}

	if (this->color.r >0.99)
	{
		this->rForward = -1;
	}

	if (this->color.b < 0.01)
	{
		this->bForward = 1;
	}

	if (this->color.g < 0.01)
	{
		this->gForward = 1;
	}

	if (this->color.r < 0.01)
	{
		this->rForward = 1;
	}
	
}