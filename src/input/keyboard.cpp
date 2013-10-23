#include "i_input.h"
#include "main/window.h"

namespace{
	// static functions for glfw*Callback-s
	boost::function<void(int,int,int,int)> gKeyCallback;
	void Keyboard_KeyCallback(GLFWwindow *, int Key, int Scan, int Action, int Mods)
	{
		if(!gKeyCallback.empty())
			gKeyCallback(Key,Scan,Action,Mods);
	}
	Keyboard::UniCharFunctor gUniCharFunctor;
	void Keyboard_UniCharCallback(GLFWwindow *, unsigned int Codepoint)
	{
		if(!gUniCharFunctor.empty())
			gUniCharFunctor(Codepoint);
	}
}

Keyboard::Keyboard()
{
	GLFWwindow* Wnd=Window::Get().GetWindow();
	gKeyCallback=boost::bind(&Keyboard::OnKey,this,_1,_2,_3,_4);
	glfwSetKeyCallback(Wnd,&Keyboard_KeyCallback);
	glfwSetCharCallback(Wnd,&Keyboard_UniCharCallback);
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
	gUniCharFunctor=Functor;
}

void Keyboard::ClearCallback( int Key )
{
	mFunctors.erase(Key);
}

