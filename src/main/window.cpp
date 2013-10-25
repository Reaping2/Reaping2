#include "window.h"

namespace {
	void Window_FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height)
	{
		EventServer<WindowResizeEvent>::Get().SendEvent(WindowResizeEvent(Width,Height));
	}
}

bool Window::Create(const uint32_t Width, const uint32_t Height, const std::string& Title)
{
	if(!glfwInit())
		return false;
	mWindow=glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL);
	if(mWindow)
	{
		glfwSetFramebufferSizeCallback(mWindow,&Window_FramebufferSizeCallback);
		glfwMakeContextCurrent(mWindow);
	}
	else
		glfwTerminate();

	return !!mWindow;
}

void Window::Destroy()
{
	if(mWindow)
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}
	mWindow=NULL;
}

bool Window::Run()
{
	if(!mWindow||glfwWindowShouldClose(mWindow))
		return false;

	glfwSwapBuffers(mWindow);
	glfwPollEvents();
	return true;
}

Window::Window()
: mWindow(NULL)
{

}

Window::~Window()
{
	Destroy();
}

void Window::Resize(const uint32_t Width, const uint32_t Height)
{
	if(!mWindow) return;
	glfwSetWindowSize(mWindow,Width,Height);
}

void Window::GetWindowSize( int& Width, int& Height ) const
{
	if(mWindow)
		glfwGetFramebufferSize(mWindow, &Width, &Height);
	else
		Width=Height=0;
}

