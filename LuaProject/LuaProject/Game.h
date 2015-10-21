#ifndef GAME_H
#define GAME_H

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
	GameObject* spinner = 0;
	vector<GameObject*> allObjects;

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