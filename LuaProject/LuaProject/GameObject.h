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
};

struct MomentInfo
{
	float acceleration;
	float velocity;
	float mass;
	float inertia;
};

//-----NOTIS----
//Rotation positiv = uppåt från högra sidan, tänk som enhetscirkeln
//Rotation negativ = neråt från högra sidan.

class GameObject
{
private:
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

	void updateUV(float dx, float dy);

	void updateUV(float angle);
	void move(float dx, float dy);
	void rotate(float dv);

	void setInertia();
protected:
	void emptyTexture();
	bool loadBMP(string name);
public:
	void move(float dx, float dy);
	GameObject();
	GameObject(vec2 pos, string texName, float sizeX, float sizeY, string type);
	void getUV(vec2 toFill[]);
	GLuint getText();
	vec2 getOrigPos();
	void setPos(vec2 pos);
	Geometry getGeoInfo(){return gInfo;}

	void applyForce(vec2 F);
	void applyMoment(float M);
	void update();

	ForceInfo getForceInfo(){return fInfo;}
	void setForceInfo(ForceInfo fI);
};

#endif