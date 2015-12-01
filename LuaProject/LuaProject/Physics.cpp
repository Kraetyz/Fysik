#include "Physics.h"

Physics* Physics::singleton = 0;

float Physics::bumperBonus = 0.0f;

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

void Physics::setBumper(float f)
{
	bumperBonus = f;
}

void Physics::gravity(GameObject* obj)
{
	obj->applyForce(vec2(0, -0.00982*obj->getForceInfo().mass));
}

void Physics::collideSphereSphere(GameObject* sph1, GameObject* sph2, float dt)
{
	Geometry oG1 = sph1->getGeoInfo();
	Geometry oG2 = sph2->getGeoInfo();
	ForceInfo oI1 = sph1->getForceInfo();
	ForceInfo oI2 = sph2->getForceInfo();

	//http://www.vobarian.com/collisions/2dcollisions2.pdf
	//Changed this implementation. The initial implenentation stopped working at some point, so now I use a modified version of the sphere vs rect reflect thing
	vec2 oPos1 = oG1.getPos();
	vec2 oPos2 = oG2.getPos();
	vec2 normal = vec2(oPos2.x - oPos1.x, oPos2.y - oPos1.y);
	vec2 unitVec = normal / (sqrt(normal.x*normal.x + normal.y*normal.y));

	vec2 v1new = normalize(reflect(oI1.velocity, unitVec));
	float v1plus = -((oI1.mass - oI2.mass*(oI1.elasticity+bumperBonus)) / (oI1.mass + oI2.mass))*length(oI1.velocity); //Elasticity +bumperBonus for fun

	oI1.velocity = v1new*v1plus;

	sph1->setPos(vec2(oPos1.x + oI1.velocity.x*dt, oPos1.y + oI1.velocity.y*dt));
	sph1->setForceInfo(oI1);
}

void Physics::collideSphereRect(GameObject* sph, GameObject* rect, float dt)
{
	Geometry sG = sph->getGeoInfo();
	Geometry rG = rect->getGeoInfo();
	ForceInfo sI = sph->getForceInfo();
	ForceInfo rI = rect->getForceInfo();

	vec2 rCorners[4];
	rect->getUV(rCorners); //Get the corners of the rectangle, which coincide with its UV coords :^)

	int corner1 = -1;
	int corner2 = -1;

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
		{
			boxNormal = vec2(0, 1);
			corner1 = NW;
			corner2 = NE;
		}
		else if (circleToRectSpace.y < -rCorners[SW].y) //collides at bottom
		{
			boxNormal = vec2(0, -1);
			corner1 = SW;
			corner2 = SE;
		}
		else //collides at left
		{
			boxNormal = vec2(-1, 0);
			collidesFromSides = true;
			corner1 = NW;
			corner2 = SW;
		}
	}
	else //right of rect center
	{
		if (circleToRectSpace.y > -rCorners[NE].y) //collides at top
		{
			boxNormal = vec2(0, 1);
			corner1 = NW;
			corner2 = NE;
		}
		else if (circleToRectSpace.y < -rCorners[SE].y) //collides at bottom
		{
			boxNormal = vec2(0, -1);
			corner1 = SW;
			corner2 = SE;
		}
		else //collides at right
		{
			boxNormal = vec2(1, 0);
			collidesFromSides = true;
			corner1 = NE;
			corner2 = SE;
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

	vec2 v1new = normalize(reflect(sI.velocity, realBoxNormal));

	float v1plus = ((sI.mass - rI.mass*sI.elasticity) / (sI.mass + rI.mass))*length(sI.velocity); //Calculate speed as if one-dimensional collision

	vec2 origVel = sI.velocity;

	sI.velocity = -v1new*v1plus;

	sph->setPos(vec2(sPos.x + sI.velocity.x*dt, sPos.y + sI.velocity.y*dt));
	
	sph->setForceInfo(sI);

	float interPoint = getIntersectPoint(sph, rect, corner1, corner2, origVel);

	if (interPoint >= 0 && interPoint <= 1)
	{
		angularToLinearVelocity(sph, rect, corner1, corner2, interPoint);
		
	}
}

float Physics::getIntersectPoint(GameObject* sph, GameObject* rect, int corner1, int corner2, vec2 sphVel)
{
	vec2 sphPos1 = sph->getGeoInfo().getPos();
	vec2 sphPos2 = sphPos1 + sphVel;

	vec2 corners[4];
	rect->getUV(corners);
	vec2 sphVec = sphPos2 - sphPos1;
	vec2 rectVec = corners[corner2] - corners[corner1];

	vec2 intSecPoint = vec2(0, 0);

	float intVal1 = -1;
	float intVal2 = -1;

	vec2 val1 = sphPos2 - corners[corner2];

	float fval1 = (val1.x * rectVec.y) - (val1.y * rectVec.x);

	float fval2 = (val1.x * sphVec.y) - (val1.y * sphVec.x);

	float fdifVal = (rectVec.x * sphVec.y) - (rectVec.y * sphVec.x);

	if (fdifVal != 0)
	{
		intVal1 = fval1 / fdifVal;
		intVal2 = fval2 / fdifVal;
	}

	if ((intVal1 >= 0 && intVal1 <= 1) && (intVal2 >= 0 && intVal2 <= 1))
	{
		intSecPoint = sphPos2 + (fval1 * sphVec);
	}

	return fval2 + sph->getGeoInfo().getRadius();

}

void Physics::angularToLinearVelocity(GameObject* sph, GameObject* rect, int corner1, int corner2, float intersectVal)
{
	vec2 corners[4];
	rect->getUV(corners);

	vec2 line = corners[corner2] - corners[corner1];

	float lineLength = length(line);

	if (intersectVal > (lineLength / 2))
	{
		float r = intersectVal - (lineLength / 2);

		float velocity = r * rect->getMomentInfo().velocity;

		velocity = abs(velocity);

		vec2 velocityVec(0, 0);
		if (sph->getForceInfo().velocity.x > 0)
			velocityVec.x += velocity;
		else
			velocityVec.x -= velocity;
		if (sph->getForceInfo().velocity.y > 0)
			velocityVec.y += velocity;
		else
			velocityVec.y -= velocity;

		sph->getGeoInfo().setPos(vec2(sph->getGeoInfo().getPos().x + velocityVec.x, sph->getGeoInfo().getPos().y + velocityVec.y));
		ForceInfo fI = sph->getForceInfo();
		fI.velocity += velocityVec;
		sph->setForceInfo(fI);
	}

}



void Physics::collideRectRect(GameObject* rect1, GameObject* rect2, float dt)
{
}

void Physics::collide(GameObject* obj1, GameObject* obj2, float dt)
{
	//collideSphereSphere(obj1, obj2);
	Geometry oI1 = obj1->getGeoInfo();
	Geometry oI2 = obj2->getGeoInfo();

	if (oI1.getRadius() > 0.0f && oI2.getRadius() > 0.0f)
		collideSphereSphere(obj1, obj2, dt);
	else if(oI1.getRadius() < 0.0f && oI2.getRadius() < 0.0f)
		collideRectRect(obj1, obj2, dt);
	else
	{
		if (oI1.getRadius() < 0.0f)
			collideSphereRect(obj2, obj1, dt);
		else
			collideSphereRect(obj1, obj2, dt);
	}
}