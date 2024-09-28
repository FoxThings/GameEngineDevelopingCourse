#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <Window/IWindow.h>
#include <Game.h>
#include <array.h>
#include <iostream>
#include <filesystem>
#include <ini.h>
#include <map>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

static const std::filesystem::path CONFIG_PATH = "../../../../../Assets/config.ini";

bool WindowsMessageLoop()
{
	MSG msg = { 0 };

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == (WM_QUIT | WM_CLOSE))
		{
			return false;
		}
	}

	return msg.message != (WM_QUIT | WM_CLOSE);
}

GameEngine::KeyCode getKeyCode(const char* value)
{
	static const std::map<std::string, GameEngine::KeyCode> keymapDict =
	{
		{"left", GameEngine::KeyCode::LEFT},
		{"right", GameEngine::KeyCode::RIGHT},
		{"up", GameEngine::KeyCode::UP},
		{"down", GameEngine::KeyCode::DOWN}
	};

	std::string strValue(value);
	if (keymapDict.contains(strValue)) {
		return keymapDict.at(strValue);
	}

	return GameEngine::KeyCode::A;
}

int handler(void* user, const char* section, const char* name,
	const char* value)
{
	GameEngine::GameKeymap* keymap = (GameEngine::GameKeymap*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("movement", "left")) {
		keymap->left = getKeyCode(value);
	}
	else if (MATCH("movement", "right")) {
		keymap->right = getKeyCode(value);
	}
	else if (MATCH("movement", "up")) {
		keymap->up = getKeyCode(value);
	}
	else if (MATCH("movement", "down")) {
		keymap->down = getKeyCode(value);
	}
	else {
		return 0;  /* unknown section/name, error */
	}

	return 1;
}

GameEngine::GameKeymap ReadLayoutConfig()
{
	GameEngine::GameKeymap keymap;

	if (ini_parse("test.ini", handler, &keymap) < 0) {
		assert(!"Can not load ini config");
	}

	return keymap;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameEngine::Core::g_MainWindowsApplication = new GameEngine::Core::Window();
	GameEngine::Core::g_MainWindowsApplication->Init(hInstance);

	std::unique_ptr<GameEngine::Game> game = std::make_unique<GameEngine::Game>(&WindowsMessageLoop);

	game->Run();

	return 0;
}