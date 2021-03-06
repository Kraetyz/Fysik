#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <gl/glew.h>
#include <gl/GL.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/transform.hpp"
#include <string>
#include <vector>
#include "Geometry.h"

#define NW 0
#define SW 1
#define NE 2
#define SE 3

using namespace glm;
using namespace std;

struct ForceInfo
{
	vec2 acceleration;
	vec2 velocity;
	float mass;
	float elasticity;
};

struct MomentInfo
{
	float acceleration;
	float velocity;
	float mass;
	float inertia;
};

//-----NOTIS----
//Rotation positiv = upp�t fr�n h�gra sidan, t�nk som enhetscirkeln. Moturs, Albin. Det heter moturs.
//Rotation negativ = ner�t fr�n h�gra sidan.

class GameObject
{
private:
	GameObject* lastCollided = 0;

	vec2 UV[4];
	float sizeX, sizeY;
	string clr;
	vec2 origPos;

	GLuint tex;

	Geometry gInfo;
	ForceInfo fInfo;
	MomentInfo mInfo;

	std::vector<vec2> forces;
	std::vector<float> moments;

	void updateUV(float dx, float dy);//Updates UV POSITION

	void updateUV(float angle); //Updates UV ROTATION
	
	void rotate(float dv, float dt);

	void setInertia();
protected:
	void emptyTexture();
	bool loadBMP(string name);
public:
	GameObject();
	GameObject(vec2 pos, string texName, float sizeX, float sizeY, string type);
	void getUV(vec2 toFill[]);
	GLuint getText();
	void reset();
	void stop();
	void setPos(vec2 pos);
	Geometry getGeoInfo(){return gInfo;}
	void move(float dx, float dy, float dt);
	void applyForce(vec2 F);
	void applyMoment(float M);
	void update(float dT);

	void setAngle(float angle);
	void setMass(float m);
	void setElasticity(float e);

	ForceInfo getForceInfo(){return fInfo;}
	void setForceInfo(ForceInfo fI);

	MomentInfo getMomentInfo(){ return mInfo; }
	void setMomentInfo(MomentInfo mI);

	bool checkIfLastCollided(GameObject* obj);
};

#endif