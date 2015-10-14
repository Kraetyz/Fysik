#ifndef PHYSICS_H
#define PHYSICS_H

#include "GameObject.h"

class Physics
{
private:
	static Physics* singleton;
	Physics();
public:
	static Physics* getPhysics();

	void gravity(GameObject* obj);
};

#endif