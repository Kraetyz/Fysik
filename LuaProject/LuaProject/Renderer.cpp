#include "Renderer.h"

Renderer::Renderer()
{
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
	
	glBindTexture(GL_TEXTURE_2D, toRender->getText());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDeleteBuffers(1, &buf);
	glDeleteVertexArrays(1, &vao);
}

void Renderer::Render(GameObject* toRender, GameObject* player)
{
	GLuint prog = shader->getProgram();
	GLuint pos = glGetUniformLocation(prog, "radius");
	glUniform1f(pos, radius);
	pos = glGetUniformLocation(prog, "backColor");
	glUniform3fv(pos, 1, &backColor[0]);
	if (player)
	{
		player->getCorners(playerCorners);
		playerPos.x = playerCorners[0].x + playerCorners[1].x + playerCorners[2].x + playerCorners[3].x;
		playerPos.x = playerPos.x / 4;
		playerPos.y = playerCorners[0].y + playerCorners[1].y + playerCorners[2].y + playerCorners[3].y;
		playerPos.y = playerPos.y / 4;
		
		GLuint playerPosID = glGetUniformLocation(prog, "playerPos");
		glUniform2fv(playerPosID, 1, &playerPos[0]);
	}	

	vec2 corners[4];
	toRender->getCorners(corners);
	vec3 color = toRender->getColor();

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

	GLuint colorID = glGetUniformLocation(prog, "color");
	glUniform3fv(colorID, 1, &color[0]);
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