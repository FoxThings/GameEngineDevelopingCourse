#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>

#include <KeyCode.h>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc,
		GameKeymap keymap
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		this->keymap = keymap;

		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		// How many objects do we want to create
		for (int i = 0; i < 1; ++i)
		{
			m_Objects.push_back(new GameObject());
			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}
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

			Update(dt);
			
			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();

			m_renderThread->OnEndFrame();
		}
	}

	void Game::Update(float dt)
	{
		const float speed = 10.;

		float xDir = GetKey(this->keymap.left) ? -1 :
			GetKey(this->keymap.right) ? 1 : 0;

		float yDir = GetKey(this->keymap.down) ? 1 :
			GetKey(this->keymap.up) ? -1 : 0;

		Math::Vector3f pos = m_Objects[0]->GetPosition();
		pos.x += speed * xDir * dt;
		pos.y -= speed * yDir * dt;

		m_Objects[0]->SetPosition(pos, m_renderThread->GetMainFrame());
	}

	bool Game::GetKey(KeyCode code)
	{
		return GameEngine::Core::g_MainWindowsApplication->IsKeyPressed(code);
	}
}
