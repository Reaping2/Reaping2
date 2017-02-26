#ifndef INCLUDED_MAIN_WINDOW_H
#define INCLUDED_MAIN_WINDOW_H

#include "platform/i_platform.h"
#include "engine/system.h"

struct WindowResizeEvent : public Event
{
    const int Width;
    const int Height;
    WindowResizeEvent( int W, int H ): Width( W ), Height( H ) {}
};

namespace engine {
class WindowSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( WindowSystem )
    bool Create( const std::string& Title );
    void Resize( const uint32_t Width, const uint32_t Height );
    void Destroy();
    void GetWindowSize( int& Width, int& Height )const;
    void Close();
    //hack
    GLFWwindow* GetWindow()
    {
        return mWindow;
    }
    virtual void Init();
    virtual void Update( double DeltaTime );
    WindowSystem();
private:
    ModelValue mExitModel;
    bool mExit;
    GLFWwindow* mWindow;

    ~WindowSystem();
};
} // namespace engine
#endif//INCLUDED_MAIN_WINDOW_H
