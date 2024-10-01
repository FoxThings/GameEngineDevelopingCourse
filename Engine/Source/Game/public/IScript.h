#pragma once
#include <GameObject.h>

namespace GameEngine
{
	class IScript
	{
	public:
		virtual void Update(float dt, size_t frame) = 0;
	};
}
