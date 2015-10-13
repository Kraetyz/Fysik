#ifndef PHYSICS_H
#define PHYSICS_H

class Physics
{
private:
	static Physics* singleton;
	Physics();
public:
	static Physics* getPhysics();
};

#endif