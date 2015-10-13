#ifndef BUTTON_H
#define BUTTON_H

#include "GameObject.h"
#include <string>

using namespace std;

class Button : public GameObject
{
private:
	string name;
public:
	Button();
	Button(vec2 pos, vec2 size, string name);
	string getName();
};

#endif