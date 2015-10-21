#ifndef GAME_H
#define GAME_H

#include "Lua/lua.hpp"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"

#include <iostream>
#include <vector>

#include "State.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Physics.h"

using namespace std;

class Game : public State
{
private:
	GameObject* player = 0;
	vector<GameObject*> allObjects;

	lua_State* L = 0;
	//lua_State* map = 0;
	int luaErrorHandlerPos = 0;

	void collide(GameObject* player);

	void findClosestObject();
public:
	Game();
	~Game();
	void Render();
	string update();
	void restart();
	void loadMap();
};

#endif