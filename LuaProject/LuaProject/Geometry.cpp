#include "Geometry.h"
#include "GameObject.h"

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

bool Geometry::checkCollision(GameObject* myObj, GameObject* aObj)
{
	Geometry aGeom = aObj->getGeoInfo();
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
				return BoxOnBoxColl(myPos, aGeom.myPos, myWidth, aGeom.myWidth, myHeight, aGeom.myHeight, myAngle, aGeom.myAngle);
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
				glm::vec2 corners[4];
				aObj->getUV(corners);
				return BoxOnSphereColl(corners, myPos, myRadius);
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
			glm::vec2 corners[4];
			myObj->getUV(corners);
			return BoxOnSphereColl(corners, aGeom.myPos, aGeom.myRadius);
		}
	}
	//if we get to this point, one of both are illegal
	return 0;

}

#pragma region internal collision

bool Geometry::BoxOnBoxColl(glm::vec2 aPos1, glm::vec2 aPos2, float aWidth1, float aWidth2, float aHeight1, float aHeight2, float aAngle1, float aAngle2)
{
	return 0;
	//we start by getting the corner positiions of our boxes
	glm::vec2 box1Corners[4];
	glm::vec2 box2Corners[4];

	box1Corners[0] = glm::vec2(-aWidth1 / 2, -aHeight1 / 2);
	box1Corners[1] = glm::vec2(-aWidth1 / 2, aHeight1 / 2);
	box1Corners[2] = glm::vec2(aWidth1 / 2, -aHeight1 / 2);
	box1Corners[3] = glm::vec2(aWidth1 / 2, aHeight1 / 2);

	box2Corners[0] = glm::vec2(-aWidth2 / 2, -aHeight2 / 2);
	box2Corners[1] = glm::vec2(-aWidth2 / 2, aHeight2 / 2);
	box2Corners[2] = glm::vec2(aWidth2 / 2, -aHeight2 / 2);
	box2Corners[3] = glm::vec2(aWidth2 / 2, aHeight2 / 2);

	/*
	CORNER INFO:
	0 = UL
	1 = LL
	2 = UR
	3 = LR
	*/

	//Praise be unto him
	//http://www.gamedev.net/page/resources/_/technical/game-programming/2d-rotated-rectangle-collision-r2604
	for (int i = 0; i < 4; i++)
	{
		float tempX = box1Corners[i].x;
		float tempY = box1Corners[i].y;
		box1Corners[i].x = tempX * glm::cos(aAngle1) - tempY * glm::sin(aAngle1);
		box1Corners[i].y = tempX * glm::sin(aAngle1) + tempY * glm::cos(aAngle1);

		box1Corners[i] += aPos1;

		tempX = box2Corners[i].x;
		tempY = box2Corners[i].y;
		box2Corners[i].x = tempX * glm::cos(aAngle2) - tempY * glm::sin(aAngle2);
		box2Corners[i].y = tempX * glm::sin(aAngle2) + tempY * glm::cos(aAngle2);

		box2Corners[i] += aPos2;
	}

	//define our 4 axises for the collision
	glm::vec2 axis[4];
	axis[0] = glm::vec2(box1Corners[2].x - box1Corners[0].x, box1Corners[2].y - box1Corners[0].y);
	axis[1] = glm::vec2(box1Corners[2].x - box1Corners[3].x, box1Corners[2].y - box1Corners[3].y);
	axis[2] = glm::vec2(box2Corners[0].x - box2Corners[1].x, box2Corners[0].y - box2Corners[1].y);
	axis[3] = glm::vec2(box2Corners[0].x - box2Corners[2].x, box2Corners[0].y - box2Corners[2].y);


	/*
	CORNER INFO:
	0 = UL
	1 = LL
	2 = UR
	3 = LR
	*/

	float minA = FLT_MAX, minB = FLT_MAX, maxA = -FLT_MAX, maxB = -FLT_MAX;

	for (int a = 0; a < 4; a++)
	{
		//a is for axii
		//resets the max/min for the next axis
		minA = minB = FLT_MAX;
		maxA = maxB = -FLT_MAX;

		for (int r1 = 0; r1 < 4; r1++)
		{
			//c1 = corners for rect 1
			//calcs max and min on projected axis

			float projAxis = box1Corners[r1].x * axis[a].x + box1Corners[r1].y * axis[a].y;
			projAxis = projAxis / (axis[a].x * axis[a].x + axis[a].y * axis[a].y);

			float projY = projAxis * axis[a].y;
			float projX = projAxis * axis[a].x;

			float toScalar = projX + projY; // projX * axis[a].x + projY * axis[a].y;
			if (toScalar >= maxA)
				maxA = toScalar;
			else if (toScalar <= minA)
				minA = toScalar;
		}

		for (int r2 = 0; r2 < 4; r2++)
		{
			//c1 = corners for rect 1
			//calcs max and min on projected axis

			float projAxis = box2Corners[r2].x * axis[a].x + box2Corners[r2].y * axis[a].y;
			projAxis = projAxis / (axis[a].x * axis[a].x + axis[a].y * axis[a].y);

			float projY = projAxis * axis[a].y;
			float projX = projAxis * axis[a].x;

			float toScalar = projX + projY; // projX * axis[a].x + projY * axis[a].y;
			if (toScalar >= maxB)
				maxB = toScalar;
			else if (toScalar <= minB)
				minB = toScalar;
		}

		//This check will return 0 if the collision doesn't happen
		if (minB > maxA && maxB < minA)
			return 0;
	}

	return 1;
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

bool Geometry::BoxOnSphereColl(glm::vec2 aBoxCorners[], glm::vec2 aSpherePos, float aRadius)
{
	glm::vec2 circleToRectSpace = aSpherePos;
	/*
	box2Corners[i].x = tempX * glm::cos(aAngle2) - tempY * glm::sin(aAngle2);
	box2Corners[i].y = tempX * glm::sin(aAngle2) + tempY * glm::cos(aAngle2);*/

	glm::vec2 boxCorners[4];

	for (int i = 0; i < 4; i++)
	{
		boxCorners[i] = aBoxCorners[i] - aSpherePos;
	}

	glm::vec2 lines[4];

	lines[0] = boxCorners[1] - boxCorners[0];
	lines[1] = boxCorners[2] - boxCorners[0];
	lines[2] = boxCorners[1] - boxCorners[3];
	lines[3] = boxCorners[2] - boxCorners[3];

	float boxXMax, boxXMin, boxYMax, boxYMin;
	boxXMax = boxYMax = -FLT_MAX;
	boxXMin = boxYMin = FLT_MAX;

	for (int i = 0; i < 4; i++)
	{
		if (aBoxCorners[i].x >= boxXMax)
			boxXMax = aBoxCorners[i].x;
		if (aBoxCorners[i].x <= boxXMin)
			boxXMin = aBoxCorners[i].x;
		if (aBoxCorners[i].y >= boxYMax)
			boxYMax = aBoxCorners[i].y;
		if (aBoxCorners[i].y <= boxYMin)
			boxYMin = aBoxCorners[i].y;
	}

	if (aSpherePos.x < boxXMax + aRadius && aSpherePos.x > boxXMin - aRadius
		&& aSpherePos.y < boxYMax + aRadius && aSpherePos.y > boxYMin - aRadius)
	{

		for (int i = 0; i < 4; i++)
		{

			float a = (lines[i].x * lines[i].x) + (lines[i].y * lines[i].y);
			float b = 2 * ((lines[i].x * boxCorners[i].x) + (lines[i].y * boxCorners[i].y));
			float c = (boxCorners[i].x * boxCorners[i].x) + (boxCorners[i].y * boxCorners[i].y) - (aRadius * aRadius);

			float delta = b * b - (4 * a * c);
			if (delta > 0)
				return 1;
		}
	}
	return 0;

	/*
	circleToRectSpace.x -= aBoxPos.x;
	circleToRectSpace.y -= aBoxPos.y;

	circleToRectSpace.x = abs(circleToRectSpace.x);
	circleToRectSpace.y = abs(circleToRectSpace.y);

	float tempX = circleToRectSpace.x;
	float tempY = circleToRectSpace.y;
	circleToRectSpace.x = tempX * glm::cos(aBoxAngle) - tempY * glm::sin(aBoxAngle);
	circleToRectSpace.y = tempX * glm::sin(aBoxAngle) + tempY * glm::cos(aBoxAngle);

	//Now we just calculate AABB vs Circle using circleToRectSpace

	if (circleToRectSpace.x > (aWidth / 2 + aRadius))
		return 0;
	if (circleToRectSpace.y > (aHeight / 2 + aRadius))
		return 0;

	if (circleToRectSpace.x <= (aWidth / 2))
		return 1;
	if (circleToRectSpace.y <= (aHeight / 2))
		return 1;

	float cornerDistSq = ((circleToRectSpace.x - aWidth / 2) * (circleToRectSpace.x - aWidth / 2))
		+ ((circleToRectSpace.y - aHeight / 2) * (circleToRectSpace.y - aHeight / 2));

	return (cornerDistSq <= (aRadius * aRadius));*/
}

#pragma endregion

#pragma endregion


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

float Geometry::getAngle()
{
	return myAngle;
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

bool Geometry::setAngle(float angle)
{
	myAngle = angle;
	return 1;
}
#pragma endregion