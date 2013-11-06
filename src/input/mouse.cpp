#include "i_input.h"
#include "main/window.h"

Mouse::Mouse()
: mHeldButtons(0)
, mMousePressEventWait(0)
, mMousePressEventRepeatInterval(0.1)
{
	Window& Wind(Window::Get());
	GLFWwindow* Wnd=Wind.GetWindow();
	glfwSetCursorPosCallback(Wnd,&Mouse::OnMouseMove);
	glfwSetMouseButtonCallback(Wnd,&Mouse::OnMouseButton);
	glfwSetCursorEnterCallback(Wnd,&Mouse::OnMouseEnter);
	glfwSetScrollCallback(Wnd,&Mouse::OnMouseScroll);
}

void Mouse::OnMouseMove( GLFWwindow* Wnd, double x, double y )
{
	Mouse& M=Get();
	M.mRawMouseCoord.x=(float)x;
	M.mRawMouseCoord.y=(float)y;
	EventServer<ScreenMouseMoveEvent>::Get().SendEvent(ScreenMouseMoveEvent(M.mRawMouseCoord));
}

void Mouse::OnMouseButton(GLFWwindow*, int button, int action, int mods)
{
	Get().MousePressed(button,action,mods);
}

void Mouse::MousePressed(int button, int action, int /*mods*/)
{
	Button_t Button=Button_Left;
	switch(button){
			case GLFW_MOUSE_BUTTON_MIDDLE:	Button=Button_Middle;	break;
			case GLFW_MOUSE_BUTTON_RIGHT:	Button=Button_Right;	break;
			default: break;
	}
	if(action==GLFW_PRESS)
	{
		mRawPressCoord=mRawMouseCoord;
		mHeldButtons|=1<<Button;
		mMousePressEventWait=mMousePressEventRepeatInterval;
		EventServer<ScreenMousePressEvent>::Get().SendEvent(ScreenMousePressEvent(mRawMouseCoord,Button));
	}
	else if(action==GLFW_RELEASE)
	{
		mHeldButtons&=~(1<<Button);
		EventServer<ScreenMouseReleaseEvent>::Get().SendEvent(ScreenMouseReleaseEvent(mRawMouseCoord,Button));
		const glm::vec2 Diff=mRawMouseCoord-mRawPressCoord;
		const float DistSqr=glm::dot(Diff,Diff);
		static const float MinDistForDrag=25.f;
		glm::vec2 Out;
		if(DistSqr>=MinDistForDrag)
			EventServer<ScreenMouseDragEvent>::Get().SendEvent(ScreenMouseDragEvent(mRawPressCoord,mRawMouseCoord,Button));
	}
}

bool Mouse::IsButtonPressed( Button_t Button ) const
{
	return !!mHeldButtons&(1<<Button);
}

void Mouse::Update(double Seconds)
{
	if(mMousePressEventWait>Seconds)
	{
		mMousePressEventWait-=Seconds;
		return;
	}
	for(size_t i=0;i<Num_Buttons;++i)
	{
		const Button_t Button((Button_t)i);
		if(IsButtonPressed(Button))
			EventServer<ScreenMousePressEvent>::Get().SendEvent(ScreenMousePressEvent(mRawMouseCoord,Button));
	}
	mMousePressEventWait=mMousePressEventRepeatInterval;
}

void Mouse::OnMouseEnter( GLFWwindow*, int entered )
{
	EventServer<MouseEnterEvent>::Get().SendEvent(MouseEnterEvent(entered==GL_TRUE));
}

void Mouse::OnMouseScroll( GLFWwindow*, double x_offs, double y_offs )
{
	// itt nincs space transition, az offsetek a gorgotol fuggnek csak
	EventServer<MouseScrollEvent>::Get().SendEvent(MouseScrollEvent(glm::vec2(x_offs,y_offs)));
}


MouseMoveEvent::~MouseMoveEvent()
{

}

MousePressEvent::~MousePressEvent()
{

}

MouseReleaseEvent::~MouseReleaseEvent()
{

}

MouseDragEvent::~MouseDragEvent()
{

}
