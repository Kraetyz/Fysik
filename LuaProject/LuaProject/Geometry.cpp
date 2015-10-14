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

#pragma endregion

#pragma region collision

bool Geometry::checkCollision(Geometry aGeom)
{
	//TODO: Redo the if-statements, there are more optimal ways to write this

	//aGeom is not a sphere
	if (aGeom.myRadius == -1)
	{
		//this is not a sphere
		if (myRadius == -1)
		{
			//both are boxes
			if (aGeom.myHeight != -1 && aGeom.myWidth != -1 && myWidth != -1 && myHeight != -1)
			{
				return BoxOnBoxColl(myPos, aGeom.myPos, myWidth, aGeom.myWidth, myHeight, aGeom.myHeight);
			}
			//one or both is not a box, and one or both is illegal
			return 0;
		}
		//this is a sphere
		else
		{
			//aGeom is a box
			if (aGeom.myHeight != -1 && aGeom.myWidth != -1)
			{
				return BoxOnSphereColl(aGeom.myPos, myPos, aGeom.myWidth, aGeom.myHeight, myRadius);
			}
		}

		//if we somehow get to this point, one of them is illegal
		return 0;
	}
	//aGeom is a sphere
	else if (aGeom.myRadius >= 0)
	{
		//this is a sphere
		if (myRadius >= 0)
		{
			return SphereOnSphereColl(myPos, aGeom.myPos, myRadius, aGeom.myRadius);
		}
		//this is a box
		else if (myWidth > 0 && myHeight > 0)
		{
			return BoxOnSphereColl(myPos, aGeom.myPos, myWidth, myHeight, aGeom.myRadius);
		}
	}
	//if we get to this point, one of both are illegal
	return 0;

}

#pragma region internal collision

bool Geometry::BoxOnBoxColl(glm::vec2 aPos1, glm::vec2 aPos2, float aWidth1, float aWidth2, float aHeight1, float aHeight2)
{

	return 0;
}

bool Geometry::SphereOnSphereColl(glm::vec2 aPos1, glm::vec2 aPos2, float aRadius1, float aRadius2)
{
	float dx = aPos1.x - aPos2.x;
	float dy = aPos1.y - aPos2.y;

	//gets absolute value
	float distance = sqrt((dx * dx) + (dy * dy));

	//collision
	if (distance < aRadius1 + aRadius2)
	{
		return 1;
	}

	return 0;
}

bool Geometry::BoxOnSphereColl(glm::vec2 aBoxPos, glm::vec2 aSpherePos, float aWidth, float aHeight, float aRadius)
{
	return 0;
}

#pragma endregion

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