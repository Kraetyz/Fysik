#include "Renderer.h"

Renderer::Renderer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader = new ShaderHolder();
	playerCorners[0] = playerCorners[1] = playerCorners[2] = playerCorners[3] = vec2(0, 0);
	radius = 1.0f;
}

Renderer::~Renderer()
{
	delete shader;
}

void Renderer::Render(Button* toRender)
{
	vec2 corners[4];
	toRender->getCorners(corners);

	GLuint buf;
	glGenBuffers(1, &buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, corners, GL_STATIC_DRAW);

	//define vertex data layout
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0); //the vertex attribute object will remember its enabled attributes
	GLuint prog = shader->getButtonProgram();
	GLuint vertexPos = glGetAttribLocation(prog, "vertex_position");
	glVertexAttribPointer(vertexPos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	

	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, ((GameObject*)toRender)->getText());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDeleteBuffers(1, &buf);
	glDeleteVertexArrays(1, &vao);
}

void Renderer::Render(GameObject* toRender)
{
	GLuint prog = shader->getProgram();

	vec2 corners[4];
	toRender->getCorners(corners);

	GLuint buf;
	glGenBuffers(1, &buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, corners, GL_STATIC_DRAW);

	//define vertex data layout
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0); //the vertex attribute object will remember its enabled attributes
	
	GLuint vertexPos = glGetAttribLocation(prog, "vertex_position");
	glVertexAttribPointer(vertexPos, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, toRender->getText());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDeleteBuffers(1, &buf);
	glDeleteVertexArrays(1, &vao);
}

void Renderer::setProgram()
{
	glUseProgram(shader->getProgram());
}

void Renderer::setBtnProgram()
{
	glUseProgram(shader->getButtonProgram());
}

void Renderer::setRadius(float r)
{
	radius = r;
}

void Renderer::setClearColor(float r, float g, float b)
{
	backColor = vec3(r, g, b);
	glClearColor(r, g, b, 1);
}