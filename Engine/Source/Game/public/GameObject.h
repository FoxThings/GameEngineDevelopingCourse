#pragma once

#include <RenderObject.h>
#include <RenderThread.h>
#include <Vector.h>
#include <IScript.h>

namespace GameEngine
{
	class GameObject final
	{
	public:
		GameObject() = default;

	public:
		Render::RenderObject** GetRenderObjectRef() { return &m_RenderObject; }

		void SetPosition(Math::Vector3f position, size_t frame)
		{
			m_Position = position;

			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(position, frame);
			}
		}

		Math::Vector3f GetPosition()
		{
			return m_Position;
		}

		void Update(float dt, size_t frame)
		{
			if (this->m_script != nullptr)
			{
				this->m_script->Update(dt, frame);
			}
		}

		void AttachScript(GameEngine::IScript* script = NULL)
		{
			this->m_script = script;
		};

	protected:
		Render::RenderObject* m_RenderObject = nullptr;

		Math::Vector3f m_Position = Math::Vector3f::Zero();

		GameEngine::IScript* m_script = nullptr;
	};
}
