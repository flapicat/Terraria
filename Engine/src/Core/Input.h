#pragma once
#include <unordered_map>
#include "Keys.h"

namespace Engine 
{
	class Input
	{
	public:
		static bool IsKeyPressed(Key k);
		static bool OnKeyPressed(Key k);
		static bool IsKeyReleased(Key k);
		static bool OnKeyReleased(Key k);
	private:
		static std::unordered_map<Key, bool> s_KeyState;
	};
}