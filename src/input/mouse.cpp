#include "i_input.h"
#include "main/window.h"

Mouse::Mouse()
: mWidth(0)
, mHeight(0)
{
	Window& Wind(Window::Get());
	GLFWwindow* Wnd=Wind.GetWindow();
	glfwSetCursorPosCallback(Wnd,&Mouse::OnMouseMove);
	mWindowResizeId=EventServer<WindowResizeEvent>::Get().Subscribe(boost::bind(&Mouse::OnWindowResizeEvent,this,_1));
	int w,h;
	Wind.GetWindowSize(w,h);
	Resize(w,h);
}

void Mouse::Resize(int Width, int Height)
{
	mWidth=Width;
	mHeight=Height;
}

void Mouse::OnWindowResizeEvent(const WindowResizeEvent& Event)
{
	Resize(Event.Width,Event.Height);
}

void Mouse::OnMouseMove( GLFWwindow* Wnd, double x, double y )
{
	Mouse& M=Get();
	if(M.mWidth==0||M.mHeight==0) return;
	const int d=std::min<int>(M.mWidth,M.mHeight);
	EventServer<MouseMoveEvent>::Get().SendEvent(MouseMoveEvent((2*x-M.mWidth)/d,(M.mHeight-2*y)/d));
}

