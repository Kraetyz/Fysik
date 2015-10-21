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
	obj->applyForce(vec2(0, -0.0000982*obj->getForceInfo().mass));
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

	v1normal = (v1normal*(oI1.mass - oI2.mass) + 2 * oI2.mass*v2normal) / (oI1.mass + oI2.mass);

	vec2 v1n = unitVec*v1normal;
	vec2 v1t = unitVec*v1tangent;

	vec2 v1new = v1n + v1t;

	oI1.velocity = -v1new;

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
	circleToRectSpace.x = tempX * glm::cos(2.0*3.141592654 - angle) - tempY * glm::sin(2.0*3.141592654 - angle);
	circleToRectSpace.y = tempX * glm::sin(2.0*3.141592654 - angle) + tempY * glm::cos(2.0*3.141592654 - angle);

	for (int c = 0; c < 4; c++)
	{
		rCorners[c] -= rPos;
		tempX = rCorners[c].x;
		tempY = rCorners[c].y;
		rCorners[c].x = tempX * glm::cos(2.0*3.141592654 - angle) - tempY * glm::sin(2.0*3.141592654 - angle);
		rCorners[c].y = tempX * glm::sin(2.0*3.141592654 - angle) + tempY * glm::cos(2.0*3.141592654 - angle);
	}

	vec2 boxNormal = vec2(0, 0);
	bool collidesFromSides = false;

	if (circleToRectSpace.x < 0.0f) //if less than zero, to left of rect center
	{	
		if (circleToRectSpace.y > -rCorners[NW].y) //collides at top
			boxNormal = vec2(0, 1);
		else if (circleToRectSpace.y < -rCorners[SW].y) //collides at bottom
			boxNormal = vec2(0, -1);
		else //collides at left
		{
			boxNormal = vec2(-1, 0);
			collidesFromSides = true;
		}
	}
	else //right of rect center
	{
		if (circleToRectSpace.y > -rCorners[NE].y) //collides at top
			boxNormal = vec2(0, 1);
		else if (circleToRectSpace.y < -rCorners[SE].y) //collides at bottom
			boxNormal = vec2(0, -1);
		else //collides at right
		{
			boxNormal = vec2(1, 0);
			collidesFromSides = true;
		}
	}

	/*

	//tempX = boxNormal.x; tempY = boxNormal.y;
	//boxNormal.x = tempX * glm::cos(angle) - tempY * glm::sin(angle);
	//boxNormal.y = tempX * glm::sin(angle) + tempY * glm::cos(angle);
	//boxNormal = normalize(boxNormal);

	//vec2 v1new = reflect(sI.velocity, boxNormal);
	//v1new = normalize(v1new);

	/*vec2 convertedSpeed;
	convertedSpeed.x = sI.velocity.x * glm::cos(2.0*3.141592654 - angle) - sI.velocity.y * glm::sin(2.0*3.141592654 - angle);
	convertedSpeed.y = sI.velocity.x * glm::sin(2.0*3.141592654 - angle) + sI.velocity.y * glm::cos(2.0*3.141592654 - angle);

	vec2 v1new = reflect(convertedSpeed, boxNormal);
	v1new = normalize(v1new);

	tempX = v1new.x;
	tempY = v1new.y;
	v1new.x = tempX * glm::cos(angle) - tempY * glm::sin(angle);
	v1new.y = tempX * glm::sin(angle) + tempY * glm::cos(angle);

	if (collidesFromSides)
		v1new.x = -v1new.x;
	else
		v1new.y = -v1new.y;

	*/

	vec2 realBoxNormal;
	realBoxNormal.x = boxNormal.x * glm::cos(angle) - boxNormal.y * glm::sin(angle);
	realBoxNormal.y = boxNormal.x * glm::sin(angle) + boxNormal.y * glm::cos(angle);

	vec2 v1new = -normalize(normalize(sI.velocity) - realBoxNormal);
	
	v1new = normalize(reflect(sI.velocity, realBoxNormal));

	float v1plus = ((sI.mass - rI.mass) / (sI.mass + rI.mass))*length(sI.velocity); //Calculate speed as if one-dimensional collision

	sI.velocity = -v1new*v1plus;

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