#ifndef INCLUDED_MAIN_WINDOW_H
#define INCLUDED_MAIN_WINDOW_H

#include "platform/i_platform.h"
#include <GLFW/glfw3.h>

struct WindowResizeEvent : public Event
{
	const int Width;
	const int Height;
	WindowResizeEvent(int W, int H):Width(W),Height(H){}
};

class Window : public Singleton<Window>
{
public:
	bool Create(const uint32_t Width, const uint32_t Height, const std::string& Title);
	void Resize(const uint32_t Width, const uint32_t Height);
	bool Run();
	void Destroy();
	void GetWindowSize(int& Width, int& Height)const;
	//hack
	GLFWwindow* GetWindow() {return mWindow;}
private:
	GLFWwindow * mWindow;

	friend class Singleton<Window>;
	Window();
	~Window();
};

#endif//INCLUDED_MAIN_WINDOW_H
