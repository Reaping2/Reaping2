#include "i_input.h"
#include "main/window.h"

Mouse::Mouse()
: mWidth(0)
, mHeight(0)
, mHeldButtons(0)
, mMousePressEventWait(0)
, mMousePressEventRepeatInterval(0.1)
{
	Window& Wind(Window::Get());
	GLFWwindow* Wnd=Wind.GetWindow();
	glfwSetCursorPosCallback(Wnd,&Mouse::OnMouseMove);
	glfwSetMouseButtonCallback(Wnd,&Mouse::OnMouseButton);
	glfwSetCursorEnterCallback(Wnd,&Mouse::OnMouseEnter);
	glfwSetScrollCallback(Wnd,&Mouse::OnMouseScroll);
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
	M.mRawMouseCoord.x=(float)x;
	M.mRawMouseCoord.y=(float)y;
	if(M.ProjectionCoordToWorldCoord(M.mRawMouseCoord,M.mMouseCoord))
		EventServer<MouseMoveEvent>::Get().SendEvent(MouseMoveEvent(M.mMouseCoord));
}

bool Mouse::ProjectionCoordToWorldCoord(const glm::vec2& In,glm::vec2& Out) const
{
	if(mWidth==0||mHeight==0) return false;
	const int d=std::min<int>(mWidth,mHeight);
	Out.x=(2*In.x-mWidth)/d;
	Out.y=(mHeight-2*In.y)/d;
	return true;
}

void Mouse::OnMouseButton(GLFWwindow*, int button, int action, int mods)
{
	Get().MousePressed(button,action,mods);
}

void Mouse::MousePressed(int button, int action, int /*mods*/)
{
	if(mWidth==0||mHeight==0)
		return;
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
		EventServer<MousePressEvent>::Get().SendEvent(MousePressEvent(mMouseCoord,Button));
	}
	else if(action==GLFW_RELEASE)
	{
		mHeldButtons&=~(1<<Button);
		EventServer<MouseReleaseEvent>::Get().SendEvent(MouseReleaseEvent(mMouseCoord,Button));
		const glm::vec2 Diff=mRawMouseCoord-mRawPressCoord;
		const float DistSqr=glm::dot(Diff,Diff);
		static const float MinDistForDrag=25.f;
		glm::vec2 Out;
		if(DistSqr>=MinDistForDrag&&ProjectionCoordToWorldCoord(mRawPressCoord,Out))
			EventServer<MouseDragEvent>::Get().SendEvent(MouseDragEvent(Out,mMouseCoord,Button));
	}
}

bool Mouse::IsButtonPressed( Button_t Button ) const
{
	return !!mHeldButtons&(1<<Button);
}

const glm::vec2& Mouse::GetMouseCoords() const
{
	return mMouseCoord;
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
			EventServer<MousePressEvent>::Get().SendEvent(MousePressEvent(mMouseCoord,Button));
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

