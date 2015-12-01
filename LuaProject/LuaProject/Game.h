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
	bool gravityOn = true;
	bool gPressed = false;
	GameObject* player = 0;
	GameObject* spinner = 0;
	vector<GameObject*> allObjects;

	void collide(GameObject* player, float dt);

	void findClosestObject();
public:
	Game();
	~Game();
	void Render();
	string update(float dT);
	void restart();
	void loadMap();
};

#endif