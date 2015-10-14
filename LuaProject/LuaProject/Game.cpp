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

	

	lua_close(L);
}

void Game::createPlayer()
{
	lua_getglobal(L, "ErrorHandler");
	luaErrorHandlerPos = lua_gettop(L);
	vec2 ret;
	lua_getglobal(L, "getObject");
	lua_pushinteger(L, -1);
	int error = lua_pcall(L, 1, 2, luaErrorHandlerPos);
	if (error)
	{
		std::cout << " ERROR: " << std::endl;
		std::cout << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
	else
	{
		ret.y = lua_tonumber(L, -1);
		lua_pop(L, 1);
		ret.x = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	player = new GameObject(ret, "HEIL.bmp", 0.8, 0.8);
	lua_pop(L, 1);
}

void Game::createObject(int index)
{
	lua_getglobal(L, "ErrorHandler");
	luaErrorHandlerPos = lua_gettop(L);
	vec2 scale;
	vec2 pos;
	vec3 col = vec3(0.5, 0.5, 0.5);
	lua_getglobal(L, "getObject");
	lua_pushinteger(L, index);
	int error = lua_pcall(L, 1, 5, luaErrorHandlerPos);
	if (error)
	{
		std::cout << " ERROR: " << std::endl;
		std::cout << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
	else
	{
		scale.y = lua_tonumber(L, -1);
		lua_pop(L, 1);
		scale.x = lua_tonumber(L, -1);
		lua_pop(L, 1);
		std::string color = lua_tostring(L, -1);
		if (color == "red")
			col = vec3(1, 0, 0);
		if (color == "green")
			col = vec3(0, 1, 0);
		if (color == "blue")
			col = vec3(0, 0, 1);
		if (color == "white")
			col = vec3(1, 1, 1);
		if (color == "black")
			col = vec3(0, 0, 0);
		lua_pop(L, 1);
		pos.y = lua_tonumber(L, -1);
		lua_pop(L, 1);
		pos.x = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	allObjects[index] = new GameObject(pos, "HEIL.bmp", scale.x, scale.y);
	lua_pop(L, 1);
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
	vec2 corners[4];
	player->getCorners(corners);
	vec2 tempCorners[4] = { vec2(corners[0]), vec2(corners[1]), vec2(corners[2]), vec2(corners[3]) };

	float playerCentX = (corners[0].x + corners[1].x + corners[2].x + corners[3].x) / 4.0;
	float playerCentY = (corners[0].y + corners[1].y + corners[2].y + corners[3].y) / 4.0;
	player->updateGeoInfo(playerCentX, playerCentY);

	if (GetKeyState('R') && GetAsyncKeyState('R'))
	{
		if (luaL_loadfile(L, "testscript.txt") || lua_pcall(L, 0, 0, luaErrorHandlerPos))
		{
			std::cout << " ERROR: " << std::endl;
			std::cout << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		if (luaL_loadfile(L, "map.txt") || lua_pcall(L, 0, 0, luaErrorHandlerPos))
		{
			std::cout << " ERROR: " << std::endl;
			std::cout << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		delete player;
		createPlayer();
		for (int c = 0; c < nrOfObjects; c++)
		{
			delete allObjects[c];
			createObject(c);
		}
	}

	float oldX, oldY;
	float newX, newY;
	if (GetKeyState('A') && GetAsyncKeyState('A'))
	{
		for (int c = 0; c < 4; c++)
		{
			lua_getglobal(L, "move");
			lua_pushnumber(L, corners[c].x);
			lua_pushstring(L, "left");
			error = lua_pcall(L, 2, 1, luaErrorHandlerPos);
			if (error)
			{
				std::cout << " ERROR: " << std::endl;
				std::cout << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			else
			{
				newX = lua_tonumber(L, -1);
				lua_pop(L, 1);
				player->moveX(newX, c);
			}
		}
	}
	if (GetKeyState('D') && GetAsyncKeyState('D'))
	{
		for (int c = 0; c < 4; c++)
		{
			lua_getglobal(L, "move");
			lua_pushnumber(L, corners[c].x);
			lua_pushstring(L, "right");
			error = lua_pcall(L, 2, 1, luaErrorHandlerPos);
			if (error)
			{
				std::cout << " ERROR: " << std::endl;
				std::cout << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			else
			{
				newX = lua_tonumber(L, -1);
				lua_pop(L, 1);
				player->moveX(newX, c);
			}
		}
		
	}

	player->getCorners(corners);
	if (collide(player->getGeoInfo()))
	{
		for (int c = 0; c < 4; c++)
		{
			player->moveX(tempCorners[c].x, c);
			float playerCentX = (tempCorners[0].x + tempCorners[1].x + tempCorners[2].x + tempCorners[3].x) / 4.0;
			float playerCentY = (tempCorners[0].y + tempCorners[1].y + tempCorners[2].y + tempCorners[3].y) / 4.0;
			player->updateGeoInfo(playerCentY, playerCentX);
		}
	}

	if (GetKeyState('W') && GetAsyncKeyState('W'))
	{
		for (int c = 0; c < 4; c++)
		{
			lua_getglobal(L, "move");
			lua_pushnumber(L, corners[c].y);
			lua_pushstring(L, "up");
			error = lua_pcall(L, 2, 1, luaErrorHandlerPos);
			if (error)
			{
				std::cout << " ERROR: " << std::endl;
				std::cout << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			else
			{
				newY = lua_tonumber(L, -1);
				lua_pop(L, 1);
				player->moveY(newY, c);
			}
		}
	}
	if (GetKeyState('S') && GetAsyncKeyState('S'))
	{
		for (int c = 0; c < 4; c++)
		{
			lua_getglobal(L, "move");
			lua_pushnumber(L, corners[c].y);
			lua_pushstring(L, "down");
			error = lua_pcall(L, 2, 1, luaErrorHandlerPos);
			if (error)
			{
				std::cout << " ERROR: " << std::endl;
				std::cout << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			else
			{
				newY = lua_tonumber(L, -1);
				lua_pop(L, 1);
				player->moveY(newY, c);
			}
		}
	}

	player->getCorners(corners);
	if (collide(player->getGeoInfo()))
	{
		for (int c = 0; c < 4; c++)
		{
			player->moveY(tempCorners[c].y, c);
			float playerCentX = (tempCorners[0].x + tempCorners[1].x + tempCorners[2].x + tempCorners[3].x) / 4.0;
			float playerCentY = (tempCorners[0].y + tempCorners[1].y + tempCorners[2].y + tempCorners[3].y) / 4.0;
			player->updateGeoInfo(playerCentY, playerCentX);
		}
	}
	lua_pop(L, 1);

	return "";
}

bool Game::collide(Geometry playerGeo)
{
	bool hit = false;
	for (int c = 0; c < nrOfObjects && !hit; c++)
	{
		Geometry g = allObjects[c]->getGeoInfo();
		hit = playerGeo.checkCollision(g);
	}
	return hit;
}

void Game::restart()
{
	vec2 oPos = player->getOrigPos();
	for (int c = 0; c < 4; c++)
	{
		player->moveX(oPos.x, c);
		player->moveY(oPos.y, c);
	}
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

		if (token == "radius")
		{
			token = "";
			ss >> token;
			float r = atof(token.c_str());
			render->setRadius(r);
		}

		else if (token == "fog")
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
			player = new GameObject(vec2(pX, pY), "ball.bmp", 0.8, 0.8);
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
				if (walls < nrOfObjects)
				{
					clr.append(".bmp");
					allObjects[walls] = new GameObject(vec2(wX, wY), clr, 1.0, 1.0);
					walls++;
				}
			}
		}
	}
	in.close();
}