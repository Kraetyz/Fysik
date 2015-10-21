#include "Game.h"
#include "Windows.h"
#include <fstream>
#include <sstream>
#include <string>

Game::Game()
{
	loadMap();
}

Game::~Game()
{
	delete player;
	delete spinner;
	for (int c = 0; c < allObjects.size(); c++)
	{
		delete allObjects[c];
	}

	Physics::release();
}

void Game::Render()
{
	render->setProgram();
	render->Render(player);
	render->Render(spinner);
	for (int c = 0; c < allObjects.size(); c++)
	{
		render->Render(allObjects[c]);
	}
}

string Game::update()
{
	if (GetKeyState('R') && GetAsyncKeyState('R'))
	{
		ifstream ballData;
		istringstream ss;
		string line;
		string token;
		float spinM;
		ballData.open("mapdata.txt");
		while (getline(ballData, line))
		{
			ss = istringstream(line);
			ss >> token;
			if (token == "ballmass")
			{
				ss >> token;
				player->setMass(atof(token.c_str()));
			}
			if (token == "spinner")
			{
				ss >> token;
				spinner->stop();
				spinner->applyMoment(atof(token.c_str()));
			}
		}
		ballData.close();
	}

	if (GetKeyState('P') && GetAsyncKeyState('P'))
		player->reset();

	vec2 pPlayer;
	pPlayer = player->getGeoInfo().getPos();
	float pForce = 0.00982;

	if (GetKeyState('A') && GetAsyncKeyState('A'))
		player->applyForce(vec2(-pForce, 0));
	if (GetKeyState('D') && GetAsyncKeyState('D'))
		player->applyForce(vec2(pForce, 0));

	if (GetKeyState('W') && GetAsyncKeyState('W'))
		player->applyForce(vec2(0, pForce));
	if (GetKeyState('S') && GetAsyncKeyState('S'))
		player->applyForce(vec2(0, -pForce));

	Physics* p = Physics::getPhysics();
	p->gravity(player);

	collide(player);

	player->update();
	spinner->update();

	for (int c=0; c<allObjects.size(); c++)
		allObjects[c]->update();

	return "";
}

void Game::collide(GameObject* player)
{
	findClosestObject();
	Geometry playerGeo = player->getGeoInfo();
	Physics* p = Physics::getPhysics();
	bool hit = false;
	
	if (playerGeo.checkCollision(player, spinner))
	{
		if (!player->checkIfLastCollided(spinner))
		{
			Physics* p = Physics::getPhysics();
			p->collide(player, spinner);
		}
		return;
	}

	for (int c = 0; c < allObjects.size() && !hit; c++)
	{
		hit = playerGeo.checkCollision(player, allObjects[c]);
		if (hit)
		{
			if (!player->checkIfLastCollided(allObjects[c]))
			{
				p->collide(player, allObjects[c]);
			}
			return;
		}
	}
	player->checkIfLastCollided(nullptr);
}

void Game::restart()
{
	loadMap();
}

void Game::loadMap()
{
	ifstream in;
	in.open("Map1.txt");

	istringstream ss;
	string line;
	string token;

	while (getline(in, line))
	{
		ss = istringstream(line);
		ss >> token;

		if (token == "fog")
		{
			token = "";
			ss >> token;
			float r = atof(token.c_str());
			token = "";
			ss >> token;
			float g = atof(token.c_str());
			token = "";
			ss >> token;
			float b = atof(token.c_str());
			render->setClearColor(r, g, b);
		}

		else if (token == "player")
		{
			token = "";
			ss >> token;
			float pX = atof(token.c_str());
			token = "";
			ss >> token;
			float pY = atof(token.c_str());
			if (player)
				delete player;
			player = new GameObject(vec2(pX, pY), "ball.bmp", 0.8, 0.8, "circle");
		}
		
		else if (token == "spinner")
		{
			token = "";
			ss >> token;
			float wX = atof(token.c_str());

			token = "";
			ss >> token;
			float wY = atof(token.c_str());

			token = "";
			ss >> token;
			string clr = token;

			vec2 size;
			float angle;

			token = "";
			ss >> token;
			size.x = atof(token.c_str());
			token = "";
			ss >> token;
			size.y = atof(token.c_str());

			token = "";
			ss >> token;
			angle = atof(token.c_str());

			clr.append(".bmp");
			spinner = new GameObject(vec2(wX, wY), clr, size.x, size.y, "rectangle");
			spinner->setAngle(angle);
			spinner->setMass(6000000.0f);
		}

		else if (token == "wall")
		{
			token = "";
			ss >> token;
			float wX = atof(token.c_str());

			token = "";
			ss >> token;
			float wY = atof(token.c_str());

			token = "";
			ss >> token;
			string clr = token;

			vec2 size;
			float angle;

			token = "";
			ss >> token;
			size.x = atof(token.c_str());
			token = "";
			ss >> token;
			size.y = atof(token.c_str());

			token = "";
			ss >> token;
			angle = atof(token.c_str());

			clr.append(".bmp");
			allObjects.push_back(new GameObject(vec2(wX, wY), clr, size.x, size.y, "rectangle"));
			allObjects[allObjects.size() - 1]->setAngle(angle);
			allObjects[allObjects.size() - 1]->setMass(6000000.0f);
		}
		else if (token == "ball")
		{
			token = "";
			ss >> token;
			float wX = atof(token.c_str());

			token = "";
			ss >> token;
			float wY = atof(token.c_str());

			vec2 size;

			token = "";
			ss >> token;
			size.x = atof(token.c_str());
			token = "";
			ss >> token;
			size.y = atof(token.c_str());

			allObjects.push_back(new GameObject(vec2(wX, wY), "ball2.bmp", size.x, size.y, "circle"));
			allObjects[allObjects.size() - 1]->setMass(6000000.0f);
		}

	}
	in.close();

	ifstream mapData;
	float spinMoment = 0.0f;
	mapData.open("mapdata.txt");
	while (getline(mapData, line))
	{
		ss = istringstream(line);
		ss >> token;
		if (token == "ballmass")
		{
			ss >> token;
			player->setMass(atof(token.c_str()));
		}
		if (token == "spinner")
		{
			ss >> token;
			spinMoment = atof(token.c_str());
		}
	}
	mapData.close();
	spinner->applyMoment(spinMoment);
}

void Game::findClosestObject()
{
	vec2 pPos = player->getGeoInfo().getPos();
	int shortestIndex = 0;

	for (int c = shortestIndex; c < allObjects.size(); c++)
	{
		if (length(allObjects[c]->getGeoInfo().getPos() - pPos) < length(allObjects[shortestIndex]->getGeoInfo().getPos() - pPos))
		{
			shortestIndex = c;
		}
	}
	GameObject* temp = allObjects[0];
	allObjects[0] = allObjects[shortestIndex];
	allObjects[shortestIndex] = temp;
}