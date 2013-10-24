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
	M.mOnMouseMove(2*x/w-1,2*y/h-1);
}

