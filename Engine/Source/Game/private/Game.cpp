#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <Input/InputHandler.h>
#include <Scripts.h>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		srand(time(0));

		// How many objects do we want to create
		for (int i = 0; i < 100; ++i)
		{
			GameEngine::GameObject* obj = new GameObject();
			m_Objects.push_back(obj);
			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);

			IScript* script = nullptr;
			switch (rand() % 3)
			{
			case 0:
				script = new MovingObjectScript(obj);
				break;
			case 1:
				script = new PlayableObjectScript(obj);
				break;
			case 2:
				script = new PhyscicalObjectScript(obj);
				break;
			}
			obj->AttachScript(script);

			Math::Vector3f pos = obj->GetPosition();
			pos.x += pos.x + i * 2 * (i % 2 == 0 ? 1 : -1);
			obj->SetPosition(pos, m_renderThread->GetMainFrame());
		}

		Core::g_InputHandler->RegisterCallback("GoForward", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoBack", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();
			Core::g_InputHandler->Update();
			Core::g_MainCamera->Update(dt);

			Update(dt);

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}

	void Game::Update(float dt)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			m_Objects[i]->Update(dt, m_renderThread->GetMainFrame());
		}
	}
}
