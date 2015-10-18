#include "Game.h"
#include "Windows.h"
#include <fstream>
#include <sstream>
#include <string>

Game::Game()
{
	L = luaL_newstate();
	luaL_openlibs(L); /* opens the standard libraries */
	if (luaL_loadfile(L, "Error.lua") || lua_pcall(L, 0, 0, 0))
	{
		std::cout << "Error handler failed" << std::endl;
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
	lua_getglobal(L, "ErrorHandler");
	luaErrorHandlerPos = lua_gettop(L);

	if (luaL_loadfile(L, "testscript.txt") || lua_pcall(L, 0, 0, luaErrorHandlerPos))
	{
		std::cout << "erroooor" << std::endl;
		std::cout << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}

	loadMap();
}

Game::~Game()
{
	delete player;
	for (int c = 0; c < nrOfObjects; c++)
	{
		delete allObjects[c];
	}
	delete[]allObjects;

	Physics::release();

	lua_close(L);
}

void Game::Render()
{
	render->setProgram();
	render->Render(player);
	for (int c = 0; c < nrOfObjects; c++)
	{
		render->Render(allObjects[c]);
	}
}

string Game::update()
{
	lua_getglobal(L, "ErrorHandler");
	luaErrorHandlerPos = lua_gettop(L);
	int error = 0;
	if (GetKeyState('R') && GetAsyncKeyState('R'))
	{
		if (luaL_loadfile(L, "testscript.txt") || lua_pcall(L, 0, 0, luaErrorHandlerPos))
		{
			std::cout << " ERROR: " << std::endl;
			std::cout << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		loadMap();
	}

	lua_pop(L, 1);

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
	p->rotate(player);
	p->rotate(allObjects[2]);

	collide(player);


	player->update();
	allObjects[2]->update();

	return "";
}

void Game::collide(GameObject* player)
{
	Geometry playerGeo = player->getGeoInfo();
	bool hit = false;
	for (int c = 0; c < nrOfObjects && !hit; c++)
	{
		Geometry g = allObjects[c]->getGeoInfo();
		hit = playerGeo.checkCollision(g);
		if (hit)
		{
			Physics* p = Physics::getPhysics();
			p->collide(player, allObjects[c]);
			return;
		}
	}
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

		else if (token == "nrOfObjects")
		{
			token = "";
			ss >> token;
			int nr = atoi(token.c_str());
			if (allObjects)
			{
				for (int i = 0; i < nrOfObjects; i++)
				{
					delete allObjects[i];
				}
				delete[] allObjects;
			}

			allObjects = new GameObject*[nr];
			nrOfObjects = nr;
		}

		else if (token == "wall")
		{
			static int walls = 0;

			token = "";
			ss >> token;
			float wX = atof(token.c_str());

			token = "";
			ss >> token;
			float wY = atof(token.c_str());

			token = "";
			ss >> token;
			string clr = token;
			if (allObjects)
			{
				if (walls < nrOfObjects - 1)
				{
					clr.append(".bmp");
					allObjects[walls] = new GameObject(vec2(wX, wY), clr, 1.0, 1.0, "rectangle");
					walls++;
				}
				else if (walls < nrOfObjects)
				{
					clr.append(".bmp");
					allObjects[walls] = new GameObject(vec2(wX, wY), clr, 4.0, 1.0, "rectangle");
				}
			}
		}
	}
	in.close();
}