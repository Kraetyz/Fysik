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

void Physics::collide(GameObject* obj1, GameObject* obj2)
{
	/*ForceInfo obj1Info = obj1->getForceInfo();
	obj1Info.velocity = vec2(0, 0);
	obj1Info.acceleration = vec2(0, 0);
	obj1->setForceInfo(obj1Info);*/
	Geometry oG1 = obj1->getGeoInfo();
	Geometry oG2 = obj2->getGeoInfo();
	ForceInfo oI1 = obj1->getForceInfo();
	ForceInfo oI2 = obj2->getForceInfo();

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

	//vec2 J = oI1.mass * (v1new - oI1.velocity);w
	//vec2 F = J / (1.0f / 60.0f);

	oI1.velocity = -v1new;

	oI2.velocity = v2new;

	obj1->setPos(vec2(oPos1.x + oI1.velocity.x, oPos1.y + oI1.velocity.y));
	obj1->setForceInfo(oI1);
	//obj1->applyForce(F);
	obj2->setForceInfo(oI2);
}