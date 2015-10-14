#include "GameObject.h"

void GameObject::setPos(vec2 pos)
{
	corners[NW] = vec2(pos.x - 0.01, pos.y + 0.01);
	corners[NE] = vec2(pos.x + 0.01, pos.y + 0.01);
	corners[SW] = vec2(pos.x - 0.01, pos.y - 0.01);
	corners[SE] = vec2(pos.x + 0.01, pos.y - 0.01);
}

GameObject::GameObject()
{
}

GameObject::GameObject(vec2 pos, string texName, float sizeX, float sizeY)
{
	float ratio = 1280.0f / 768.0f;
	origPos = pos;
	pos.x = pos.x*0.064;
	pos.y = pos.y*0.064*ratio;
	corners[NW] = vec2(pos.x - sizeX*0.032, pos.y - sizeY*0.032*ratio);
	corners[NE] = vec2(pos.x + sizeX*0.032, pos.y - sizeY*0.032*ratio);
	corners[SW] = vec2(pos.x - sizeX*0.032, pos.y + sizeY*0.032*ratio);
	corners[SE] = vec2(pos.x + sizeX*0.032, pos.y + sizeY*0.032*ratio);

	loadBMP(texName);

	gInfo = Geometry(pos, 0.032);
}

void GameObject::getCorners(vec2 toFill[])
{
	toFill[NW] = corners[NW];
	toFill[NE] = corners[NE];
	toFill[SW] = corners[SW];
	toFill[SE] = corners[SE];
}

vec2 GameObject::getOrigPos()
{
	return origPos;
}

void GameObject::moveX(float newX, int index)
{
	corners[index].x = newX;
}

void GameObject::moveY(float newY, int index)
{
	corners[index].y = newY;
}

void GameObject::emptyTexture()
{
	glGenTextures(1, &tex);

	glActiveTexture(GL_TEXTURE0);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(0, tex);
}

bool GameObject::loadBMP(std::string imagepath)
{
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	FILE* file = fopen(imagepath.c_str(), "rb");
	if (!file)
		return false;

	if (fread(header, 1, 54, file) != 54) // If not 54 bytes read : problem
		return false;

	if (header[0] != 'B' || header[1] != 'M')
		return false;

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)
		dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	glGenTextures(1, &tex);

	glActiveTexture(GL_TEXTURE0);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, tex);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete[] data;

	return true;
}

GLuint GameObject::getText()
{
	return tex;
}