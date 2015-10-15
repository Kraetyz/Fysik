#include "GameObject.h"

void GameObject::setPos(vec2 pos)
{
	gInfo.setPos(pos);
	UV[NW] = vec2(pos.x - sizeX*0.032, pos.y - sizeY*0.032);
	UV[NE] = vec2(pos.x + sizeX*0.032, pos.y - sizeY*0.032);
	UV[SW] = vec2(pos.x - sizeX*0.032, pos.y + sizeY*0.032);
	UV[SE] = vec2(pos.x + sizeX*0.032, pos.y + sizeY*0.032);
}

GameObject::GameObject()
{
}

GameObject::GameObject(vec2 pos, string texName, float sX, float sY)
{
	origPos = pos;
	pos.x = pos.x*0.064;
	pos.y = pos.y*0.064;
	sizeX = sX;
	sizeY = sY;
	UV[NW] = vec2(pos.x - sizeX*0.032, pos.y - sizeY*0.032);
	UV[NE] = vec2(pos.x + sizeX*0.032, pos.y - sizeY*0.032);
	UV[SW] = vec2(pos.x - sizeX*0.032, pos.y + sizeY*0.032);
	UV[SE] = vec2(pos.x + sizeX*0.032, pos.y + sizeY*0.032);

	//loadBMP(texName);
	loadBMP("ball.bmp");

	gInfo = Geometry(pos, sizeX*0.032, sizeX * 0.032);
}

void GameObject::getUV(vec2 toFill[])
{
	toFill[NW] = UV[NW];
	toFill[NE] = UV[NE];
	toFill[SW] = UV[SW];
	toFill[SE] = UV[SE];
}

vec2 GameObject::getOrigPos()
{
	return origPos;
}

void GameObject::updateUV(float dx, float dy)
{
	for (int c = 0; c < 4; c++)
	{
		UV[c].x += dx;
		UV[c].y += dy;
	}
}

void GameObject::move(float dx, float dy)
{
	updateUV(dx, dy);
	vec2 newpos = gInfo.getPos();
	newpos.x += dx;
	newpos.y += dy;
	gInfo.setPos(newpos);
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