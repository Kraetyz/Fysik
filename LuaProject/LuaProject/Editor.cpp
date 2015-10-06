#include "Editor.h"
#include "Windows.h"
#include <fstream>
#include <sstream>

Editor::Editor()
{
	L = luaL_newstate();
	luaL_openlibs(L); // opens the standard libraries
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

	render->setRadius(20.0f);
	render->setClearColor(0.1f, 0.2f, 0.3f);
	lua_pop(L, 1);

	nrOfObjects = 0;
	objectMax = 20;
	allObjects = new GameObject*[objectMax];

	currentColor = "green";
}

Editor::~Editor()
{
	delete player;
	for (int c = 0; c < nrOfObjects; c++)
	{
		delete allObjects[c];
	}
	delete[]allObjects;



	lua_close(L);
}

void Editor::createPlayer(glm::vec2 pos)
{
	if (player)
		delete player;
	player = new GameObject(pos, "red", 0.8, 0.8);
}

void Editor::createGoal(glm::vec2 pos)
{
	if (goal)
		delete goal;
	goal = new GameObject(pos, "green", 0.4, 0.4);
}

void Editor::createObject(glm::vec2 pos, string col)
{
	allObjects[nrOfObjects] = new GameObject(pos, col, 1, 1);
	nrOfObjects++;
	if (nrOfObjects == objectMax)
		expandObjectArray();
}

void Editor::expandObjectArray()
{
	GameObject** temp = new GameObject*[objectMax + 10];
	for (int c = 0; c < nrOfObjects; c++)
	{
		temp[c] = allObjects[c];
	}
	delete[]allObjects;
	allObjects = temp;
	objectMax += 10;
}

void Editor::Render()
{
	render->setProgram();
	if (player)
		render->Render(player, player);
	for (int c = 0; c < nrOfObjects; c++)
	{
		render->Render(allObjects[c]);
	}

	if (goal)
		render->Render(goal);
}

string Editor::update()
{
	if (clickTimer > FLT_EPSILON)
		clickTimer -= 0.016;
	goalUpdate();
	return "";
}

void Editor::goalUpdate()
{
	if (goal)
		goal->updateColor();
}

void Editor::saveMap()
{
	ofstream out;
	out.open("Map1.txt");

	stringstream line;
	line << "radius 0.2";

	out << line.str() << endl;

	line = stringstream();
	line << "fog 0.1 0.2 0.3";

	out << line.str() << endl;

	float playerX;
	float playerY;

	vec2 pos;
	if (player)
	{
		pos = player->getOrigPos();

		playerX = pos.x;
		playerY = pos.y;
	}
	else
	{
		playerX = 0;
		playerY = 0;
	}
	line = stringstream();
	line << "player " << playerX << " " << playerY;

	out << line.str() << endl;

	float goalX, goalY;
	if (goal)
	{
		pos = goal->getOrigPos();

		goalX = pos.x;
		goalY = pos.y;
	}
	else
	{
		goalX = 2;
		goalY = 2;
	}
	line = stringstream();
	line << "goal " << goalX << " " << goalY;

	out << line.str() << endl;

	line = stringstream();
	line << "nrOfObjects " << nrOfObjects;
	out << line.str() << endl;

	float wX = 0, wY = 0;
	string clr;
	for (int i = 0; i < nrOfObjects; i++)
	{
		//wall - 3 4 green
		pos = allObjects[i]->getOrigPos();

		wX = pos.x;
		wY = pos.y;
		line = stringstream();
		line << "wall " << wX << " " << wY << " " << allObjects[i]->getColorString();

		out << line.str() << endl;
	}
}

void Editor::giveCursorPos(glm::vec2 pos)
{
	if (mode == 1 && clickTimer < FLT_EPSILON)
	{
		createObject(pos, currentColor);
		clickTimer = 0.3f;
	}
	if (mode == 2 && clickTimer < FLT_EPSILON)
	{
		createPlayer(pos);
		clickTimer = 0.3f;
	}
	if (mode == 3 && clickTimer < FLT_EPSILON)
	{
		createGoal(pos);
		clickTimer = 0.3f;
	}
}