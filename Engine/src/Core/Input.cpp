#include <pch.h>
#include "Input.h"
#include <GLFW/glfw3.h>

#include "App.h"

namespace Engine
{
	std::unordered_map<Key, bool> Input::s_KeyState;

	bool Input::IsKeyPressed(Key k)
	{
		auto glfwWindow = App::Get().GetWindow()->GetNativeWindow();
		return glfwGetKey(glfwWindow, static_cast<uint32_t>(k)) == GLFW_PRESS;
	}

	bool Input::OnKeyPressed(Key k)
	{
		bool isPressed = IsKeyPressed(k);
		bool& wasPressed = s_KeyState[k];

		if (isPressed && !wasPressed)
		{
			wasPressed = true;
			return true;
		}
		if (!isPressed)
			wasPressed = false;

		return false;
	}

	bool Input::IsKeyReleased(Key k)
	{
		auto glfwWindow = App::Get().GetWindow()->GetNativeWindow();
		return glfwGetKey(glfwWindow, static_cast<uint32_t>(k)) == GLFW_RELEASE;
	}

	bool Input::OnKeyReleased(Key k)
	{
		bool isPressed = IsKeyPressed(k);
		bool& wasPressed = s_KeyState[k];

		if (!isPressed && wasPressed)
		{
			wasPressed = false;
			return true;
		}

		wasPressed = isPressed;

		return false;
	}
}