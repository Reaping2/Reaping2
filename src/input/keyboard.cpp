#include "i_input.h"

void Keyboard::KeyCallback(GLFWwindow *, int Key, int Scan, int Action, int Mods)
{
	if(Action==GLFW_REPEAT)return;	// no need
	if(Key==GLFW_KEY_UNKNOWN)return;
	EventServer<KeyEvent>::Get().SendEvent(KeyEvent(Key,Mods,Action==GLFW_PRESS?KeyState::Down:KeyState::Up));
}

void Keyboard::UniCharCallback(GLFWwindow *, unsigned int Codepoint)
{
	EventServer<UniCharEvent>::Get().SendEvent(UniCharEvent(Codepoint));
}

Keyboard::Keyboard()
{
}

void Keyboard::SetWindow( GLFWwindow* Wnd )
{
	glfwSetKeyCallback(Wnd,&Keyboard::KeyCallback);
	glfwSetCharCallback(Wnd,&Keyboard::UniCharCallback);
}

