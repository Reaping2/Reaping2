#ifndef INCLUDED_INPUT_MOUSE_H
#define INCLUDED_INPUT_MOUSE_H

#include "main/window.h"

class Mouse : public Singleton<Mouse>
{
public:
	enum Button_t {
		Button_Left,
		Button_Middle,
		Button_Right,
		Num_Buttons,
	};
	bool IsButtonPressed(Button_t Button)const;
	const glm::vec2& GetMouseCoords()const;
	void Update(double Seconds);
private:
	int mWidth;
	int mHeight;
	uint32_t mHeldButtons;
	Registration mWindowResizeId;
	friend class Singleton<Mouse>;
	glm::vec2 mRawPressCoord;
	glm::vec2 mRawMouseCoord;
	glm::vec2 mMouseCoord;
	double mMousePressEventWait;
	const double mMousePressEventRepeatInterval;

	Mouse();
	bool ProjectionCoordToWorldCoord(const glm::vec2& In,glm::vec2& Out) const;
	void OnWindowResizeEvent(const WindowResizeEvent& Event);
	void Resize(int Width, int Height);
	void MousePressed(int button, int action, int mods);

	static void OnMouseMove(GLFWwindow*, double x, double y);
	static void OnMouseButton(GLFWwindow*, int button, int action, int mods);
};

struct MouseMoveEvent
{
	const glm::vec2 Pos;
	MouseMoveEvent(const glm::vec2 P):Pos(P){}
};

struct MousePressEvent
{
	const glm::vec2 Pos;
	const Mouse::Button_t Button;
	MousePressEvent(const glm::vec2& P, Mouse::Button_t B):Pos(P),Button(B){}
};

struct MouseReleaseEvent
{
	const glm::vec2 Pos;
	const Mouse::Button_t Button;
	MouseReleaseEvent(const glm::vec2& P, Mouse::Button_t B):Pos(P),Button(B){}
};

struct MouseDragEvent
{
	const glm::vec2 From;
	const glm::vec2 To;
	const Mouse::Button_t Button;
	MouseDragEvent(const glm::vec2& F, const glm::vec2& T, Mouse::Button_t B):From(F),To(T),Button(B){}
};

struct MouseLeaveEvent
{

};

struct MouseScrollEvent
{

};

#endif//INCLUDED_INPUT_MOUSE_H
