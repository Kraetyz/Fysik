#include "ShaderHolder.h"

ShaderHolder::ShaderHolder()
{
	const char* vertex_shader = R"(
		#version 400
		layout(location = 0) in vec2 vertex_position;
		out vec2 position;

		void main ()
		{
			gl_Position = vec4(vertex_position, 1.0, 1.0);
			position = vec2(gl_Position.x, gl_Position.y);
		}
	)";

	const char* fragment_shader = R"(
		#version 400
		out vec4 fragment_color;
		uniform vec3 color;
		in vec2 position;

		uniform vec2 playerPos;
		uniform float radius;
		uniform vec3 backColor;

		void main ()
		{
			float ratio = 1280.0/768.0;
			vec2 dist = playerPos - position;
			dist.y = dist.y/ratio;
			if (length(dist) < radius)
			{
				fragment_color = vec4(color, 1.0);
			}
			else
			{
				fragment_color = vec4(backColor, 1);
			}
		}
	)";
	//create vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, nullptr);
	glCompileShader(vs);

	//create fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, nullptr);
	glCompileShader(fs);

	//link shader program (connect vs and ps)
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	const char* btn_vertex_shader = R"(
		#version 400
		layout(location = 0) in vec2 vertex_position;
		out vec2 UV;

		void main ()
		{
			if (gl_VertexID == 0)
			{
				UV = vec2(0,1);
			}
			if (gl_VertexID == 1)
			{
				UV = vec2(0,0);
			}
			if (gl_VertexID == 2)
			{
				UV = vec2(1,1);
			}
			if (gl_VertexID == 3)
			{
				UV = vec2(1,0);
			}
			gl_Position = vec4(vertex_position, 1.0, 1.0);
		}
	)";

	const char* btn_fragment_shader = R"(
		#version 400
		out vec4 fragment_color;
		uniform sampler2D texSampler;
		in vec2 UV;
		
		void main ()
		{
			fragment_color = texture(texSampler, vec2(UV.s, 1-UV.t));
			//fragment_color = vec4(1, 0, 1, 1);
		}
	)";

	//create vertex shader
	GLuint bvs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(bvs, 1, &btn_vertex_shader, nullptr);
	glCompileShader(bvs);

	//create fragment shader
	GLuint bfs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(bfs, 1, &btn_fragment_shader, nullptr);
	glCompileShader(bfs);

	//link shader program (connect vs and ps)
	buttonprog = glCreateProgram();
	glAttachShader(buttonprog, bvs);
	glAttachShader(buttonprog, bfs);
	glLinkProgram(buttonprog);
}

GLuint ShaderHolder::getProgram()
{
	return program;
}

GLuint ShaderHolder::getButtonProgram()
{
	return buttonprog;
}