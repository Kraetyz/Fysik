#include "GameObject.h"

void GameObject::setPos(vec2 pos)
{
	gInfo.setPos(pos);
	UV[NW] = vec2(pos.x - sizeX*0.032, pos.y - sizeY*0.032);
	UV[NE] = vec2(pos.x + sizeX*0.032, pos.y - sizeY*0.032);
	UV[SW] = vec2(pos.x - sizeX*0.032, pos.y + sizeY*0.032);
	UV[SE] = vec2(pos.x + sizeX*0.032, pos.y + sizeY*0.032);
}

void GameObject::setAngle(float angle)
{
	gInfo.setAngle(angle);
	updateUV(angle);
}

GameObject::GameObject()
{
}

GameObject::GameObject(vec2 pos, string texName, float sX, float sY, string type)
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
	loadBMP(texName);

	if (type == "circle")
		gInfo = Geometry(pos, sizeX*0.032);
	if (type == "rectangle")
		gInfo = Geometry(pos, sizeX*0.064, sizeY*0.064);
	fInfo.velocity = vec2(0,0);
	fInfo.acceleration = vec2(0,0);
	fInfo.mass = 50.0f; //SAMMA SOM MINFO MASSAN PLS ÄNDRA BÄGGE

	mInfo.acceleration = 0;
	mInfo.velocity = 0;
	mInfo.mass = 50.0f; //SAMMA SOM FINFO MASSAN PLS ÄNDRA BÄGGE

	setInertia();
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

void GameObject::setInertia()
{
	float width = gInfo.getWidth();
	float height = gInfo.getHeight();
	float rad = gInfo.getRadius();


	float inertia = 0;
	if (rad == -1 && width > 0 && height > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			inertia = (mInfo.mass / 4) * (((height / 2) * (height / 2)) + ((width / 2) * width / 2));
		}
	}

	else if (rad > 0 && width == -1 && height == -1)
	{
		inertia = mInfo.mass * rad * rad;
	}

	mInfo.inertia = inertia;
}

void GameObject::updateUV(float dx, float dy)
{
	for (int c = 0; c < 4; c++)
	{
		UV[c].x += dx;
		UV[c].y += dy;
	}
}

void GameObject::updateUV(float angle)
{
	
	float tempX = 0, tempY = 0;
	vec2 boxcorners[4];
	
	boxcorners[0] = UV[0] - gInfo.getPos();
	boxcorners[1] = UV[1] - gInfo.getPos();
	boxcorners[2] = UV[2] - gInfo.getPos();
	boxcorners[3] = UV[3] - gInfo.getPos();
	for(int i = 0; i < 4; i++)
	{
		//boxcorners[i].x -= 300;
		//boxcorners[i].y -= 300;
		//boxcorners[i].x /= 300;
		//boxcorners[i].y /= 300;
		tempX = boxcorners[i].x;
		tempY = boxcorners[i].y;
		boxcorners[i].x = tempX * glm::cos(angle) - tempY * glm::sin(angle);
		boxcorners[i].y = tempX * glm::sin(angle) + tempY * glm::cos(angle);

		boxcorners[i] += gInfo.getPos();
		UV[i] = boxcorners[i];
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

void GameObject::rotate(float dv)
{
	gInfo.setAngle(gInfo.getAngle() + dv);
	updateUV(dv);
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

void GameObject::applyForce(vec2 F)
{
	forces.push_back(F);
}

void GameObject::applyMoment(float M)
{
	moments.push_back(M);
}

void GameObject::update()
{
	vec2 resultantF = vec2(0, 0);
	for (int c = 0; c < forces.size(); c++)
	{
		resultantF += forces[c];
	}
	forces.clear();
	vec2 a = resultantF / fInfo.mass;
	fInfo.acceleration = a;
	fInfo.velocity += fInfo.acceleration;
	move(fInfo.velocity.x, fInfo.velocity.y);

	float resultantM = 0;
	for (int i = 0; i < moments.size(); i++)
	{
		resultantM += moments[i];
	}
	moments.clear();
	float alpha = resultantM / mInfo.inertia;
	mInfo.acceleration = alpha;
	mInfo.velocity += mInfo.acceleration;
	rotate(mInfo.velocity);
}

void GameObject::setForceInfo(ForceInfo fI)
{
	fInfo.acceleration = fI.acceleration;
	fInfo.velocity = fI.velocity;
}