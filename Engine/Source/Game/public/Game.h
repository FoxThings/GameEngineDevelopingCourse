#pragma once

#include <RenderEngine.h>
#include <RenderThread.h>
#include <Timer.h>
#include <Window/IWindow.h>

namespace GameEngine
{
	struct GameKeymap
	{
		KeyCode left;
		KeyCode right;
		KeyCode down;
		KeyCode up;
	};

	static GameKeymap DEFAULT_KEYMAP =
	{
		KeyCode::LEFT,
		KeyCode::RIGHT,
		KeyCode::DOWN,
		KeyCode::UP
	};

	class GameObject;

	class Game final
	{
	public:
		Game() = delete;
		Game(
			std::function<bool()> PlatformLoopFunc,
			GameKeymap keymap = DEFAULT_KEYMAP
		);

	public:
		void Run();
		void Update(float dt);
		bool GetKey(KeyCode code);

	private:
		// The main idea behind having this functor is to abstract the common code from the platfrom-specific code
		std::function<bool()> PlatformLoop = nullptr;

	private:
		Core::Timer m_GameTimer;
		std::unique_ptr<Render::RenderThread> m_renderThread;
		std::vector<GameObject*> m_Objects;
		GameKeymap keymap;
	};
}