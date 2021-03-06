#ifndef PHYSICS_H
#define PHYSICS_H

#include "GameObject.h"

class Physics
{
private:
	static Physics* singleton;
	static float bumperBonus;
	Physics();
	
	void collideSphereSphere(GameObject* sph1, GameObject* sph2, float dt);
	void collideSphereRect(GameObject* sph, GameObject* rect, float dt);
	void collideRectRect(GameObject* rect1, GameObject* rect2, float dt);

	float getIntersectPoint(GameObject* sph, GameObject* rect, int corner1, int corner2, vec2 sphVel);
	void angularToLinearVelocity(GameObject* sph, GameObject* rect, int corner1, int corner2, float intersectVal);
	
public:
	static Physics* getPhysics();
	
	static void setBumper(float f);

	void gravity(GameObject* obj);

	void collide(GameObject* obj1, GameObject* obj2, float dt);

	static void release();
};

#endif