#ifndef PHYSICS_H
#define PHYSICS_H

#include "GameObject.h"

class Physics
{
private:
	static Physics* singleton;
	Physics();
	
	void collideSphereSphere(GameObject* sph1, GameObject* sph2);
	void collideSphereRect(GameObject* sph, GameObject* rect);
	void collideRectRect(GameObject* rect1, GameObject* rect2);

	float getIntersectPoint(GameObject* sph, GameObject* rect, int corner1, int corner2, vec2 sphVel);
	void angularToLinearVelocity(GameObject* sph, GameObject* rect, int corner1, int corner2, float intersectVal);
	void linearToAngularVelocity(GameObject* sph, GameObject* rect, int corner1, int corner2, float intersectVal);
public:
	static Physics* getPhysics();

	void gravity(GameObject* obj);

	void collide(GameObject* obj1, GameObject* obj2);

	static void release();
};

#endif