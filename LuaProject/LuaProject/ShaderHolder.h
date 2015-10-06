#ifndef SHADERHOLDER_H
#define SHADERHOLDER_H

#include <gl/glew.h>
#include <gl/GL.h>

class ShaderHolder
{
private:
	GLuint program;
	GLuint buttonprog;
public:
	ShaderHolder();
	GLuint getProgram();
	GLuint getButtonProgram();
};

#endif