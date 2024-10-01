#pragma once
#include <ObjectScript.h>

namespace GameEngine
{
	class MovingObjectScript : public GameEngine::ObjectScript
	{
	public:
		const float speed = 2;
		const float timeToChangeDir = 5;

		MovingObjectScript(GameEngine::GameObject* obj) : GameEngine::ObjectScript(obj) {}

		void Update(float dt, size_t frame)
		{
			m_timer += dt;

			if (m_timer >= timeToChangeDir)
			{
				m_timer = 0;
				m_dir *= -1;
			}

			Math::Vector3f pos = m_obj->GetPosition();
			pos.z += speed * dt * m_dir;
			m_obj->SetPosition(pos, frame);
		}

	private:
		float m_timer = 0;
		float m_dir = 1;
	};

	// <------------------------------------------------------------------------------------>

	class PlayableObjectScript : public GameEngine::ObjectScript
	{
	public:
		const float speed = 2;

		PlayableObjectScript(GameEngine::GameObject* obj) : GameEngine::ObjectScript(obj)
		{
			Core::g_InputHandler->RegisterCallback("CubeRight", [&]() { this->xAxis = 1; });
			Core::g_InputHandler->RegisterCallback("CubeLeft", [&]() { this->xAxis = -1; });
		}

		void Update(float dt, size_t frame)
		{
			Math::Vector3f pos = m_obj->GetPosition();
			pos.x += speed * dt * xAxis;
			m_obj->SetPosition(pos, frame);

			xAxis = 0;
		}

	private:
		float xAxis = 0;
	};

// <------------------------------------------------------------------------------------>

	class PhyscicalObjectScript : public GameEngine::ObjectScript
	{
	public:
		const float gForce = 9;
		const float jumpForce = 10;

		PhyscicalObjectScript(GameEngine::GameObject* obj) : GameEngine::ObjectScript(obj) {}

		void Update(float dt, size_t frame)
		{
			Math::Vector3f pos = m_obj->GetPosition();
			if (pos.y <= 0)
			{
				m_velocity = jumpForce;
			}

			m_velocity -= gForce * dt;
			pos.y += m_velocity * dt;

			m_obj->SetPosition(pos, frame);
		}

	private:
		float m_velocity = 0;
	};
}
