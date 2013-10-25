#ifndef INCLUDED_INPUT_MOUSE_H
#define INCLUDED_INPUT_MOUSE_H

#include "main/window.h"

struct MouseMoveEvent
{
	const double X;
	const double Y;
	MouseMoveEvent(double x, double y):X(x),Y(y){}
};

class Mouse : public Singleton<Mouse>
{
public:
private:
	int mWidth;
	int mHeight;
	Registration mWindowResizeId;
	friend class Singleton<Mouse>;

	Mouse();
	void OnWindowResizeEvent(const WindowResizeEvent& Event);
	void Resize(int Width, int Height);
	static void OnMouseMove(GLFWwindow*, double x, double y);
};

#endif//INCLUDED_INPUT_MOUSE_H
