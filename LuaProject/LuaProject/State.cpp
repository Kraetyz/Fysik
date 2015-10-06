#include "State.h"


State::~State()
{
	delete render;
}

State::State()
{
	render = new Renderer();
}

void State::RenderButton(Button* toRender)
{
	render->setBtnProgram();
	render->Render(toRender);
}

