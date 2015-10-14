#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <gl/glew.h>
#include <gl/GL.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/transform.hpp"
#include <string>
#include "Geometry.h"

#define NW 0
#define SW 1
#define NE 2
#define SE 3

using namespace glm;
using namespace std;

class GameObject
{
private:
	vec2 corners[4];
	string clr;
	vec2 origPos;

	GLuint tex;

	Geometry gInfo;
protected:
	void setPos(vec2 pos);
	void emptyTexture();
	bool loadBMP(string name);
public:
	GameObject();
	GameObject(vec2 pos, string texName, float sizeX, float sizeY);
	void getCorners(vec2 toFill[]);
	GLuint getText();
	vec2 getOrigPos();
	void moveX(float newX, int index);
	void moveY(float newY, int index);

	Geometry getGeoInfo(){return gInfo;}
};

#endif