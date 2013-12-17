#ifndef INCLUDED_MAIN_WINDOW_H
#define INCLUDED_MAIN_WINDOW_H

#include "platform/i_platform.h"

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
	void Close();
	//hack
	GLFWwindow* GetWindow() {return mWindow;}
private:
	ModelValue mExitModel;
	bool mExit;
	GLFWwindow * mWindow;

	friend class Singleton<Window>;
	Window();
	~Window();
};

#endif//INCLUDED_MAIN_WINDOW_H
