#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/transform.hpp"

class Geometry
{
private:
	glm::vec2 myPos = glm::vec2(0, 0);
	float myRadius = -1;

	float myWidth = -1;
	float myHeight = -1;

	float myAngle = 0;

	bool BoxOnBoxColl(glm::vec2 aPos1, glm::vec2 aPos2, float aWidth1, float aWidth2, float aHeight1, float aHeight2, float aAngle1, float aAngle2);
	bool SphereOnSphereColl(glm::vec2 aPos1, glm::vec2 aPos2, float aRadius1, float aRadius2);
	bool BoxOnSphereColl(glm::vec2 aBoxPos, glm::vec2 aSpherePos, float aWidth, float aHeight, float aRadius, float aBoxAngle);
public:
	Geometry();
	Geometry(glm::vec2 aPos, float aRadius);
	Geometry(glm::vec2 aPos, float aWidth, float aHeight);
	~Geometry();

	bool checkCollision(Geometry aGeom);

	glm::vec2 getPos();
	float getRadius();
	float getWidth();
	float getHeight();
	float getAngle();

	bool setPos(glm::vec2 aPos);
	bool setRadius(float aRadius);
	bool setWidth(float aWidth);
	bool setHeight(float aHeight);
	bool setAngle(float angle);
};
#endif