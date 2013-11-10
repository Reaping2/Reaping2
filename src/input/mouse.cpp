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
		mMousePressEventWait=mMousePressEventRepeatInterval;
		EventServer<ScreenMousePressEvent>::Get().SendEvent(ScreenMousePressEvent(mRawMouseCoord,Button));
		mHeldButtons|=1<<Button;
	}
	else if(action==GLFW_RELEASE)
	{
		EventServer<ScreenMouseReleaseEvent>::Get().SendEvent(ScreenMouseReleaseEvent(mRawMouseCoord,Button));
		const glm::vec2 Diff=mRawMouseCoord-mRawPressCoord;
		const float DistSqr=glm::dot(Diff,Diff);
		static const float MinDistForDrag=25.f;
		glm::vec2 Out;
		if(DistSqr>=MinDistForDrag)
			EventServer<ScreenMouseDragEvent>::Get().SendEvent(ScreenMouseDragEvent(mRawPressCoord,mRawMouseCoord,Button));
		mHeldButtons&=~(1<<Button);
	}
	LOG("Held buttons:%d",mHeldButtons);

}

bool Mouse::IsButtonPressed( Button_t Button ) const
{
	return !(!(mHeldButtons&(1<<Button)));
}

void Mouse::Update(double Seconds)
{
	if(mMousePressEventWait>Seconds)
	{
		mMousePressEventWait-=Seconds;
		return;
	}
	for(size_t i=1;i<Num_Buttons;++i)
	{
		const Button_t Button((Button_t)i);
		if(IsButtonPressed(Button))
		{
			LOG("Held buttons and is pressed:%d,%d,%d \n",mHeldButtons,!(!mHeldButtons&(1<<Button)),Button);
			EventServer<ScreenMousePressEvent>::Get().SendEvent(ScreenMousePressEvent(mRawMouseCoord,Button));
		}
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
