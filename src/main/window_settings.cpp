#include "window_settings.h"
#include "platform/settings.h"
#include "platform/log.h"

struct WindowSettings::Impl
{
    Mode mMode = Windowed;
    glm::vec2 mSize = glm::vec2( 1280, 960 );
    Hints mHints;
    GLFWmonitor* mMonitor = nullptr;

    Impl()
    {
        InitMode();
        InitSize();
        InitHints();
        InitMonitor();
        AdjustMonitorHints();
    }

    void InitMonitor()
    {
        if( mMode == Windowed )
        {
            return;
        }

        auto* primary = glfwGetPrimaryMonitor();
        int count = 0;
        auto** monitors = glfwGetMonitors( &count );
        GLFWmonitor* largest = nullptr;
        glm::vec2 largestsize;
        GLFWmonitor* named = nullptr;
        std::string preferredMonitor = Settings::Get().GetStr( "graphics.preferred_monitor", "" );
        for( int i = 0; i < count; ++i )
        {
            auto* mon = monitors[ i ];
            std::string name = glfwGetMonitorName( mon );
            L1( "Monitor: %s\n", name.c_str() );
            if( named == nullptr && !preferredMonitor.empty() && preferredMonitor == name )
            {
                named = mon;
            }
            auto* mode = glfwGetVideoMode( mon );
            if( mode != nullptr )
            {
                if( largest == nullptr ||
                    ( mode->width > largestsize.x && mode->height > largestsize.y ) )
                {
                    largest = mon;
                    largestsize = glm::vec2( mode->width, mode->height );
                }
            }
        }
        if( named != nullptr )
        {
            mMonitor = named;
        }
        else if( primary != nullptr )
        {
            mMonitor = primary;
        }
        else if( largest != nullptr )
        {
            mMonitor = largest;
        }
        else if( count > 0 )
        {
            mMonitor = monitors[ 0 ];
        }
    }

    void AdjustMonitorHints()
    {
        if( mMonitor == nullptr || mMode != BorderlessWindowed )
        {
            return;
        }
        auto* mode = glfwGetVideoMode( mMonitor );
        if( mode != nullptr )
        {
            mSize.x = mode->width;
            mSize.y = mode->height;
            mHints[ GLFW_RED_BITS ] = mode->redBits;
            mHints[ GLFW_GREEN_BITS ] = mode->greenBits;
            mHints[ GLFW_BLUE_BITS ] = mode->blueBits;
            mHints[ GLFW_REFRESH_RATE ] = mode->refreshRate;
        }
    }

    void InitMode()
    {
        std::string windowMode = Settings::Get().GetStr( "graphics.window_mode", "windowed" );
        if( windowMode == "fullscreen" )
        {
            mMode = Fullscreen;
        }
        else if( windowMode == "borderless_windowed" )
        {
            mMode = BorderlessWindowed;
        }
    }

    void InitSize()
    {
        mSize.x = Settings::Get().GetUInt( "graphics.width", mSize.x );
        mSize.y = Settings::Get().GetUInt( "graphics.height", mSize.y );
    }

    void InitHints()
    {
        std::map<Mode,Hints> hints = {
            { Windowed, {
                              { GLFW_RESIZABLE, GL_TRUE },
                              { GLFW_DECORATED, GL_TRUE },
                              { GLFW_VISIBLE, GL_TRUE },
                          } },
            { Fullscreen, {
                              { GLFW_RESIZABLE, GL_FALSE },
                              { GLFW_DECORATED, GL_FALSE },
                              { GLFW_REFRESH_RATE, 0 },
                          } },
            { BorderlessWindowed, {
                              { GLFW_RESIZABLE, GL_FALSE },
                              { GLFW_DECORATED, GL_FALSE },
                              { GLFW_REFRESH_RATE, 0 },
                          } },
        };
        mHints = hints[ mMode ];
        mHints[ GLFW_CONTEXT_VERSION_MAJOR ] = 3;
        mHints[ GLFW_CONTEXT_VERSION_MINOR ] = 3;
        mHints[ GLFW_OPENGL_PROFILE ] = GLFW_OPENGL_CORE_PROFILE;
    }
};

WindowSettings::WindowSettings()
    : mImpl( new Impl() )
{
}

WindowSettings::~WindowSettings()
{
}

WindowSettings::Mode WindowSettings::GetMode() const
{
    return mImpl->mMode;
}

WindowSettings::Hints const& WindowSettings::GetHints() const
{
    return mImpl->mHints;
}

glm::vec2 WindowSettings::GetSize() const
{
    return mImpl->mSize;
}

GLFWmonitor* WindowSettings::GetPreferredMonitor() const
{
    return mImpl->mMonitor;
}


