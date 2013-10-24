#ifndef INCLUDED_INPUT_MOUSE_H
#define INCLUDED_INPUT_MOUSE_H

class Mouse : public Singleton<Mouse>
{
public:
	typedef boost::function<void(double,double)> MouseMoveFunc;
	void SetMouseMoveCallback(const MouseMoveFunc& Function);
private:
	Mouse();
	friend class Singleton<Mouse>;
	MouseMoveFunc mOnMouseMove;
	static void OnMouseMove(GLFWwindow*, double x, double y);
};

#endif//INCLUDED_INPUT_MOUSE_H
