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

void Physics::gravity(GameObject* obj)
{
	obj->applyForce(vec2(0, -0.00001));
}

void Physics::collide(GameObject* obj1, GameObject* obj2)
{
	ForceInfo obj1Info = obj1->getForceInfo();
	obj1Info.velocity = vec2(0, 0);
	obj1Info.acceleration = vec2(0, 0);
	obj1->setForceInfo(obj1Info);
}