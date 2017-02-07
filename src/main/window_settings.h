#ifndef INCLUDED_MAIN_WINDOW_SETTINGS_H
#define INCLUDED_MAIN_WINDOW_SETTINGS_H

#include <memory.h>
#include "platform/i_platform.h"

class WindowSettings : public platform::Singleton<WindowSettings>
{
public:
    enum Mode {
        Windowed,
        BorderlessWindowed,
        Fullscreen,
    };
    ~WindowSettings();
    Mode GetMode() const;
    GLFWmonitor* GetPreferredMonitor() const;
    glm::vec2 GetSize() const;
    typedef std::map<int, int> Hints;
    Hints const& GetHints() const;

private:
    friend class platform::Singleton<WindowSettings>;
    WindowSettings();
    struct Impl;
    std::unique_ptr<Impl> mImpl;
};

#endif // INCLUDED_MAIN_WINDOW_SETTINGS_H

