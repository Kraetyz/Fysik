#include "Physics.h"

Physics* Physics::singleton = 0;

Physics::Physics()
{
}

Physics* Physics::getPhysics()
{
	if (singleton == 0)
		singleton = new Physics();
	return singleton;
}

void Physics::release()
{
	if (singleton)
		delete singleton;
}

void Physics::gravity(GameObject* obj)
{
	obj->applyForce(vec2(0, -0.00982));
}

void Physics::collideSphereSphere(GameObject* sph1, GameObject* sph2)
{
	Geometry oG1 = sph1->getGeoInfo();
	Geometry oG2 = sph2->getGeoInfo();
	ForceInfo oI1 = sph1->getForceInfo();
	ForceInfo oI2 = sph2->getForceInfo();

	//http://www.vobarian.com/collisions/2dcollisions2.pdf
	vec2 oPos1 = oG1.getPos();
	vec2 oPos2 = oG2.getPos();
	vec2 normal = vec2(oPos2.x - oPos1.x, oPos2.y - oPos1.y);
	vec2 unitVec = normal / (sqrt(normal.x*normal.x + normal.y*normal.y));
	vec2 tangent = vec2(-unitVec.y, unitVec.x);

	float v1normal = dot(unitVec, oI1.velocity);
	float v2normal = dot(unitVec, oI2.velocity);
	float v1tangent = dot(tangent, oI1.velocity);
	float v2tangent = dot(tangent, oI2.velocity);

	float v1normalNew = (v1normal*(oI1.mass - oI2.mass) + 2 * oI2.mass*v2normal) / (oI1.mass + oI2.mass);
	float v2normalNew = (v1normal*(oI2.mass - oI1.mass) + 2 * oI1.mass*v1normal) / (oI1.mass + oI2.mass);

	vec2 v1n = unitVec*v1normalNew;
	vec2 v2n = unitVec*v2normalNew;
	vec2 v1t = unitVec*v1tangent;
	vec2 v2t = unitVec*v2tangent;

	vec2 v1new = v1n + v1t;
	vec2 v2new = v2n + v2t;

	oI1.velocity = -v1new;

	oI2.velocity = v2new;

	sph1->setPos(vec2(oPos1.x + oI1.velocity.x, oPos1.y + oI1.velocity.y));
	sph1->setForceInfo(oI1);

	sph2->setForceInfo(oI2);
}

void Physics::collideSphereRect(GameObject* sph, GameObject* rect)
{
	Geometry sG = sph->getGeoInfo();
	Geometry rG = rect->getGeoInfo();
	ForceInfo sI = sph->getForceInfo();
	ForceInfo rI = rect->getForceInfo();
	MomentInfo rM = rect->getMomentInfo();

	vec2 rCorners[4];
	rect->getUV(rCorners); //Get the corners of the rectangle, which coincide with its UV coords :^)

	vec2 sPos = sG.getPos();
	vec2 rPos = rG.getPos();

	float angle = rG.getAngle();
	glm::vec2 circleToRectSpace = sPos;
	/*
	box2Corners[i].x = tempX  glm::cos(aAngle2) - tempY  glm::sin(aAngle2);
	box2Corners[i].y = tempX  glm::sin(aAngle2) + tempY  glm::cos(aAngle2);*/

	circleToRectSpace.x -= rPos.x;
	circleToRectSpace.y -= rPos.y;

	float tempX = circleToRectSpace.x;
	float tempY = circleToRectSpace.y;
	circleToRectSpace.x = tempX * glm::cos(angle) - tempY * glm::sin(angle);
	circleToRectSpace.y = tempX * glm::sin(angle) + tempY * glm::cos(angle);

	for (int c = 0; c < 4; c++)
	{
		rCorners[c] -= rPos;
		tempX = rCorners[c].x;
		tempY = rCorners[c].y;
		rCorners[c].x = tempX * glm::cos(-angle) - tempY * glm::sin(-angle);
		rCorners[c].y = tempX * glm::sin(-angle) + tempY * glm::cos(-angle);
	}

	vec2 boxNormal = vec2(0, 0);
	bool x = false;

	if (circleToRectSpace.x < 0.0f) //if less than zero, to left of rect center
	{
		if (circleToRectSpace.y > -rCorners[NW].y) //collides at top
		{
			boxNormal = vec2(0, sPos.y - rPos.y);
		}
		else if (circleToRectSpace.y < -rCorners[SW].y) //collides at bottom
		{
			boxNormal = vec2(0, sPos.y - rPos.y);
		}
		else //collides at left
		{
			boxNormal = vec2(sPos.x - rPos.x, 0);
			x = true;
		}
	}
	else //right of rect center
	{
		if (circleToRectSpace.y > -rCorners[NE].y) //collides at top
		{
			boxNormal = vec2(0, sPos.y - rPos.y);
		}
		else if (circleToRectSpace.y < -rCorners[SE].y) //collides at bottom
		{
			boxNormal = vec2(0, sPos.y - rPos.y);
		}
		else //collides at left
		{
			boxNormal = vec2(sPos.x - rPos.x, 0);
			x = true;
		}
	}
	tempX = boxNormal.x; tempY = boxNormal.y;
	boxNormal.x = tempX * glm::cos(angle) - tempY * glm::sin(angle);
	boxNormal.y = tempX * glm::sin(angle) + tempY * glm::cos(angle);
	boxNormal = normalize(boxNormal);

	vec2 v1new = reflect(sI.velocity, boxNormal);

	sI.velocity = v1new;

	sph->setPos(vec2(sPos.x + sI.velocity.x, sPos.y + sI.velocity.y));
	sph->setForceInfo(sI);
}

void Physics::collideRectRect(GameObject* rect1, GameObject* rect2)
{
}

void Physics::collide(GameObject* obj1, GameObject* obj2)
{
	//collideSphereSphere(obj1, obj2);
	Geometry oI1 = obj1->getGeoInfo();
	Geometry oI2 = obj2->getGeoInfo();

	if (oI1.getRadius() > 0.0f && oI2.getRadius() > 0.0f)
		collideSphereSphere(obj1, obj2);
	else if(oI1.getRadius() < 0.0f && oI2.getRadius() < 0.0f)
		collideRectRect(obj1, obj2);
	else
	{
		if (oI1.getRadius() < 0.0f)
			collideSphereRect(obj2, obj1);
		else
			collideSphereRect(obj1, obj2);
	}
}