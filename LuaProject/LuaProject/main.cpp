//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014.
//--------------------------------------------------------------------------------------
#include <windows.h>

#include <gl/glew.h>
#include <gl/GL.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/transform.hpp"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#include "Game.h"
#include "Menu.h"
#include "Editor.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "Lua/lua.hpp"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"

lua_State* buttonState;
Button buttons[10];
int nrOfButtons = 0;

HWND window;
State* state;

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HGLRC CreateOpenGLContext(HWND wndHandle);

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

void SetViewport()
{
	glViewport(0, 0, 1280, 768);
}

void setupButtons()
{
	glm::vec2 northwest, southeast, pos, size;
	lua_getglobal(buttonState, "nrOfButtons");
	nrOfButtons = lua_tointeger(buttonState, -1);
	lua_pop(buttonState, 1);
	for (int c = 1; c < nrOfButtons + 1; c++)
	{
		lua_getglobal(buttonState, "getButton");
		lua_pushinteger(buttonState, c);
		lua_pcall(buttonState, 1, 5, 0);
		string buttonType = lua_tostring(buttonState, -1);
		southeast.y = lua_tonumber(buttonState, -2);
		southeast.x = lua_tonumber(buttonState, -3);
		northwest.y = lua_tonumber(buttonState, -4);
		northwest.x = lua_tonumber(buttonState, -5);
		lua_pop(buttonState, 5);
		pos = glm::vec2((northwest.x + southeast.x) / 2.0f, (northwest.y + southeast.y) / 2.0f);
		size = glm::vec2(southeast.x - northwest.x, northwest.y - southeast.y);
		buttons[c - 1] = Button(pos, size, buttonType);
	}
}

static int killThroughLua(lua_State* L)
{
	PostQuitMessage(0);
	return 0;
}

static int goToMenu(lua_State* L)
{
	((Editor*)state)->saveMap();
	delete state;
	state = new Menu();
	if (luaL_loadfile(buttonState, "menuButtons.txt") || lua_pcall(buttonState, 0, 0, 0))
		throw;
	setupButtons();
	return 0;
}

static int goToGame(lua_State* L)
{
	delete state;
	state = new Game();
	if (luaL_loadfile(buttonState, "gameButt.txt") || lua_pcall(buttonState, 0, 0, 0))
		throw;
	setupButtons();
	return 0;
}

static int goToEditor(lua_State* L)
{
	delete state;
	state = new Editor();
	if (luaL_loadfile(buttonState, "editorButtons.txt") || lua_pcall(buttonState, 0, 0, 0))
		throw;
	setupButtons();
	return 0;
}

static int restartGame(lua_State* L)
{
	((Game*)state)->restart();
	return 0;
}

static int screenClick(lua_State* L)
{
	POINT pCur;
	GetCursorPos(&pCur);
	ScreenToClient(window, &pCur);
	float mouseX = ((pCur.x - (1280 / 2)) / 640.0f) * 15;
	float mouseY = (-(pCur.y - (768 / 2)) / 366.0f) * 9;
	((Editor*)state)->giveCursorPos(glm::vec2(mouseX, mouseY));
	return 0;
}

static int setMode(lua_State* L)
{
	int mode = lua_tointeger(L, -1);
	lua_pop(L, 1);

	((Editor*)state)->setMode(mode);

	return 0;
}

static int setColor(lua_State* L)
{
	string color = lua_tostring(L, -1);
	lua_pop(L, 1);
	
	((Editor*)state)->setCurrentColor(color);

	return 0;
}

void clickUpdate()
{
	POINT pCur;
	GetCursorPos(&pCur);
	ScreenToClient(window, &pCur);
	lua_getglobal(buttonState, "clickCheck");
	lua_getglobal(buttonState, "nrOfButtons");
	int buttonNr = lua_tointeger(buttonState, -1);
	lua_pop(buttonState, 1);
	float mouseX = ((pCur.x - (1280 / 2)) / 640.0f)*15;
	float mouseY = (-(pCur.y - (768 / 2)) / 366.0f)*9;
	lua_pushnumber(buttonState, mouseX);
	lua_pushnumber(buttonState, mouseY);
	if (lua_pcall(buttonState, 2, 1, 0))
		throw;
	bool weHaveClickBoys = lua_toboolean(buttonState, -1);
	lua_pop(buttonState, 1);
}

void buttonRender()
{
	for (int c = 0; c < nrOfButtons; c++)
	{
		state->RenderButton(&buttons[c]);
	}
}

void Update()
{
	string msg = state->update();
	if (msg == "GOOOOOAAAAAAL")
	{
		delete state;
		state = new Menu();
		if (luaL_loadfile(buttonState, "menuButtons.txt") || lua_pcall(buttonState, 0, 0, 0))
			throw;
		setupButtons();
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	buttonRender();
	state->Render();
}

void registerLuaFuncs()
{
	lua_register(buttonState, "execute", killThroughLua);
	lua_register(buttonState, "startGame", goToGame);
	lua_register(buttonState, "restartGame", restartGame);
	lua_register(buttonState, "startEditor", goToEditor);
	lua_register(buttonState, "click", screenClick);
	lua_register(buttonState, "setMode", setMode);
	lua_register(buttonState, "backToMenu", goToMenu);
	lua_register(buttonState, "editorColor", setColor);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };
	window = InitWindow(hInstance); //1. Skapa fönster


	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;

	if (window)
	{
		HDC hDC = GetDC(window);
		HGLRC hRC = CreateOpenGLContext(window); //2. Skapa och koppla OpenGL context

		glewInit(); //3. Initiera The OpenGL Extension Wrangler Library (GLEW)
		//glEnable(GL_CULL_FACE);

		SetViewport(); //4. Sätt viewport

		ShowWindow(window, nCmdShow);

		state = new Menu();

		buttonState = luaL_newstate();
		luaL_openlibs(buttonState);

		if (luaL_loadfile(buttonState, "Error.lua") || lua_pcall(buttonState, 0, 0, 0))
		{
			std::cout << "Error handler failed" << std::endl;
			std::cerr << lua_tostring(buttonState, -1) << std::endl;
			lua_pop(buttonState, 1);
		}
		lua_getglobal(buttonState, "ErrorHandler");
		int luaErrorHandlerPos = lua_gettop(buttonState);

		registerLuaFuncs();

		if (luaL_loadfile(buttonState, "menuButtons.txt") || lua_pcall(buttonState, 0, 0, luaErrorHandlerPos))
		{
			std::cout << "erroooor" << std::endl;
			std::cout << lua_tostring(buttonState, -1) << std::endl;
			lua_pop(buttonState, 1);
		}
		setupButtons();
		lua_pop(buttonState, 1);

		while (WM_QUIT != msg.message)
		{
			if (msg.message == WM_LBUTTONDOWN)
				clickUpdate();
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			else
			{
				Update(); //9. Rendera
				SwapBuffers(hDC); //10. Växla front- och back-buffer
			}
		}

		wglMakeCurrent(NULL, NULL);
		ReleaseDC(window, hDC);
		wglDeleteContext(hRC);
		DestroyWindow(window);
	}

	lua_close(buttonState);

	return (int)msg.wParam;
}

HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"LUAbyrinth";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, 1280, 768 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"LUAbyrinth",
		L"LUAbyrinth",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HGLRC CreateOpenGLContext(HWND wndHandle)
{
	//get handle to a device context (DC) for the client area
	//of a specified window or for the entire screen
	HDC hDC = GetDC(wndHandle);

	//details: http://msdn.microsoft.com/en-us/library/windows/desktop/dd318286(v=vs.85).aspx
	static  PIXELFORMATDESCRIPTOR pixelFormatDesc =
	{
		sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd  
		1,                                // version number  
		PFD_DRAW_TO_WINDOW |              // support window  
		PFD_SUPPORT_OPENGL |              // support OpenGL  
		PFD_DOUBLEBUFFER |               // double buffered
		PFD_TYPE_RGBA,                    // RGBA type  
		32,                               // 32-bit color depth  
		0, 0, 0, 0, 0, 0,                 // color bits ignored  
		0,                                // no alpha buffer  
		0,                                // shift bit ignored  
		0,                                // no accumulation buffer  
		0, 0, 0, 0,                       // accum bits ignored  
		0,                                // 0-bits for depth buffer <-- modified by Stefan      
		0,                                // no stencil buffer  
		0,                                // no auxiliary buffer  
		PFD_MAIN_PLANE,                   // main layer  
		0,                                // reserved  
		0, 0, 0                           // layer masks ignored  
	};

	//attempt to match an appropriate pixel format supported by a
	//device context to a given pixel format specification.
	int pixelFormat = ChoosePixelFormat(hDC, &pixelFormatDesc);

	//set the pixel format of the specified device context
	//to the format specified by the iPixelFormat index.
	SetPixelFormat(hDC, pixelFormat, &pixelFormatDesc);

	//create a new OpenGL rendering context, which is suitable for drawing
	//on the device referenced by hdc. The rendering context has the same
	//pixel format as the device context.
	HGLRC hRC = wglCreateContext(hDC);

	//makes a specified OpenGL rendering context the calling thread's current
	//rendering context. All subsequent OpenGL calls made by the thread are
	//drawn on the device identified by hdc. 
	wglMakeCurrent(hDC, hRC);

	return hRC;
}