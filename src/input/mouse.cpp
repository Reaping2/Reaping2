#include "i_input.h"
#include "main/window.h"

void Mouse::SetMouseMoveCallback(const MouseMoveFunc& Function)
{
	mOnMouseMove=Function;
}

Mouse::Mouse()
{
	GLFWwindow* Wnd=Window::Get().GetWindow();
	glfwSetCursorPosCallback(Wnd,&Mouse::OnMouseMove);
}

void Mouse::OnMouseMove( GLFWwindow* Wnd, double x, double y )
{
	Mouse& M=Get();
	if(M.mOnMouseMove.empty()) return;
	int w,h;
	glfwGetFramebufferSize(Wnd,&w,&h);
	if(w==0||h==0) return;
	const int d=std::min<int>(w,h);
	M.mOnMouseMove((2*x-w)/d,(h-2*y)/d);
}

