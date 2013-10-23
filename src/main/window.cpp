#include "window.h"

namespace {
	std::auto_ptr<Window::ResizeCallback> gResizeCallback;
	void Window_FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height)
	{
		if(gResizeCallback.get())
			gResizeCallback->operator()(Width,Height);
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

void Window::SetResizeCallback(std::auto_ptr<ResizeCallback> Callback)
{
	gResizeCallback.reset(Callback.release());
	if(gResizeCallback.get())
	{
		int Width, Height;
		glfwGetFramebufferSize(mWindow, &Width, &Height);
		gResizeCallback->operator()(Width,Height);
	}
}

void Window::Resize(const uint32_t Width, const uint32_t Height)
{
	if(!mWindow) return;
	glfwSetWindowSize(mWindow,Width,Height);
}

