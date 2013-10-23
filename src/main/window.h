#ifndef INCLUDED_MAIN_WINDOW_H
#define INCLUDED_MAIN_WINDOW_H

#include "platform/i_platform.h"
#include <GLFW/glfw3.h>

class Window : public Singleton<Window>
{
public:
	bool Create(const uint32_t Width, const uint32_t Height, const std::string& Title);
	void Resize(const uint32_t Width, const uint32_t Height);
	bool Run();
	void Destroy();
	typedef boost::function< void(const uint32_t,const uint32_t) > ResizeCallback;
	void SetResizeCallback(std::auto_ptr<ResizeCallback> Callback);
private:

	GLFWwindow * mWindow;

	friend class Singleton<Window>;
	Window();
	~Window();
};

#endif//INCLUDED_MAIN_WINDOW_H
