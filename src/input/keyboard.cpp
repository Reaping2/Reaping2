#include "i_input.h"
#include "main/window.h"

void Keyboard::KeyCallback(GLFWwindow *, int Key, int Scan, int Action, int Mods)
{
	Get().OnKey(Key,Scan,Action,Mods);
}

void Keyboard::UniCharCallback(GLFWwindow *, unsigned int Codepoint)
{
	Keyboard& Keys=Get();
	if(!Keys.mUniCharFunctor.empty())
		Keys.mUniCharFunctor(Codepoint);
}

Keyboard::Keyboard()
{
	GLFWwindow* Wnd=Window::Get().GetWindow();
	glfwSetKeyCallback(Wnd,&Keyboard::KeyCallback);
	glfwSetCharCallback(Wnd,&Keyboard::UniCharCallback);
}

void Keyboard::SetCallback(int Key, const KeyEventFunctor& Functor)
{
	mFunctors[Key]=Functor;
}

void Keyboard::OnKey(int Key, int Scan, int Action, int Mods)
{
	if(Action==GLFW_REPEAT)return;	// no need
	if(Key==GLFW_KEY_UNKNOWN)return;
	KeyEventFunctors::const_iterator i=mFunctors.find(Key);
	if(i==mFunctors.end())return;
	const KeyEventFunctor& F=i->second;
	if(F.empty())return;
	F(Key,Mods,Action==GLFW_PRESS?KeyState::Down:KeyState::Up);
}

void Keyboard::SetCharCallback(const UniCharFunctor& Functor)
{
	mUniCharFunctor=Functor;
}

void Keyboard::ClearCallback( int Key )
{
	mFunctors.erase(Key);
}

