#ifndef RENDERER_H
#define RENDERER_H

#include "ShaderHolder.h"
#include "Button.h"

class Renderer
{
private:
	ShaderHolder* shader;
	GLuint buf;
	vec2 playerCorners[4];
	vec2 playerPos = vec2(0, 0);
	float radius;
	vec3 backColor;
public:
	Renderer();
	~Renderer();
	void Render(Button* toRender);
	void Render(GameObject* toRender, GameObject* player = 0);
	void setProgram();
	void setBtnProgram();
	void setRadius(float r);
	void setClearColor(float r, float g, float b);
};

#endif