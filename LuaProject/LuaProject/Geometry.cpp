#include"Geometry.h"

#pragma region constructors/destructor

Geometry::Geometry()
{

}

Geometry::Geometry(glm::vec2 aPos, float aRadius)
{
	myPos = aPos;
	myRadius = aRadius;
}

Geometry::Geometry(glm::vec2 aPos, float aWidth, float aHeight)
{
	myPos = aPos;
	myWidth = aWidth;
	myHeight = aHeight;
}

Geometry::~Geometry()
{

}

bool Geometry::checkCollision(Geometry aGeom)
{
	//if (aGeom);
}

#pragma endregion


#pragma region private functions

#pragma region get/set
glm::vec2 Geometry::getPos()
{
	return myPos;
}

float Geometry::getRadius()
{
	return myRadius;
}

float Geometry::getWidth()
{
	return myWidth;
}

float Geometry::getHeight()
{
	return myHeight;
}

bool Geometry::setPos(glm::vec2 aPos)
{
	myPos = aPos;
	return 1;
}

bool Geometry::setRadius(float aRadius)
{
	if (myRadius != -1 && aRadius > 0)
	{
		myRadius = aRadius;
		return 1;
	}
	
	return 0;
}

bool Geometry::setWidth(float aWidth)
{
	if (myWidth != -1 && aWidth > 0)
	{
		myWidth = aWidth;
		return 1;
	}

	return 0;
}

bool Geometry::setHeight(float aHeight)
{
	if (myHeight != -1 && aHeight > 0)
	{
		myHeight = aHeight;
		return 1;
	}

	return 0;
}
#pragma endregion

#pragma endregion