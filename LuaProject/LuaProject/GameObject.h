#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <gl/glew.h>
#include <gl/GL.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/transform.hpp"
#include <string>

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
	vec3 color;
	string clr;
	vec2 origPos;
protected:
	void setPos(vec2 pos);
	void setColor(vec3 col);
public:
	GameObject();
	GameObject(vec2 pos, string clr, float sizeX, float sizeY);
	void getCorners(vec2 toFill[]);
	vec3 getColor();
	string getColorString();
	vec2 getOrigPos();
	void updateColor();
	void goalUpdate();
	void moveX(float newX, int index);
	void moveY(float newY, int index);
	int rForward;
	int gForward;
	int bForward;
};

#endif