#pragma once
#include <GameObject.h>

namespace GameEngine
{
	class ObjectScript : public GameEngine::IScript
	{
	public:
		ObjectScript(GameEngine::GameObject* obj)
		{
			this->m_obj = obj;
		}

	protected:
		GameEngine::GameObject* m_obj;
	};
}
