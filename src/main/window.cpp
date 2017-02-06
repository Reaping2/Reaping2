#include "window.h"
#include "platform/settings.h"

namespace {
void Window_FramebufferSizeCallback( GLFWwindow* Window, int Width, int Height )
{
    EventServer<WindowResizeEvent>::Get().SendEvent( WindowResizeEvent( Width, Height ) );
}
}
namespace engine {
bool WindowSystem::Create( const std::string& Title )
{
    if( !glfwInit() )
    {
        LOG( "glfwInit failed!\n" );
        return false;
    }
    uint32_t Width = 1280;
    uint32_t Height = 960;
    Width = Settings::Get().GetUInt( "graphics.width", Width );
    Height = Settings::Get().GetUInt( "graphics.height", Height );
    std::string windowMode = Settings::Get().GetStr( "graphics.window_mode", "windowed" );
    glfwDefaultWindowHints();
    // possible modes: fullscreen_native_res, fullscreen, borderless_windowed, windowed
    std::map<std::string,std::map<int,int> > hints = {
        { "windowed", {
                          { GLFW_RESIZABLE, GL_TRUE },
                          { GLFW_DECORATED, GL_TRUE },
                          { GLFW_VISIBLE, GL_TRUE },
                      } },
        { "fullscreen", {
                          { GLFW_RESIZABLE, GL_FALSE },
                          { GLFW_DECORATED, GL_FALSE },
                          { GLFW_REFRESH_RATE, 0 },
                      } },
    };
    auto it = hints.find( windowMode );
    auto const& selectedHints = it == hints.end() ? hints[ "windowed" ] : it->second;
    for( auto const& hint : selectedHints )
    {
        glfwWindowHint( hint.first, hint.second );
    }

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    GLFWmonitor* monitor = nullptr;
    if( windowMode == "fullscreen" )
    {
        monitor = glfwGetPrimaryMonitor();
        if( monitor == nullptr )
        {
            int count = 0;
            auto** monitors = glfwGetMonitors( &count );
            if( count > 0 )
            {
                monitor = monitors[0];
            }
        }
        if( monitor != nullptr )
        {
            auto* mode = glfwGetVideoMode( monitor );
            if( mode != nullptr )
            {
                Width = mode->width;
                Height = mode->height;
            }
        }
    }
    mWindow = glfwCreateWindow( Width, Height, Title.c_str(), monitor, nullptr );
    if( mWindow )
    {
        LOG( "Window creation succeeded!\n" );
        glfwSetFramebufferSizeCallback( mWindow, &Window_FramebufferSizeCallback );
        glfwMakeContextCurrent( mWindow );
        glewExperimental = true;
        GLenum err = glewInit();
        if( err != GLEW_OK )
        {
            LOG( "glewInit failed!\n" );
            glfwTerminate();
            mWindow = NULL;
        }
        else
        {
            glfwSetInputMode( mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
            bool vsync = Settings::Get().GetBool( "graphics.vsync", false );
            glfwSwapInterval( vsync ? 1 : 0 );
        }
    }
    else
    {
        LOG( "Window creation failed!\n" );
        glfwTerminate();
    }
    return !!mWindow;
}

void WindowSystem::Destroy()
{
    if( mWindow )
    {
        glfwDestroyWindow( mWindow );
        glfwTerminate();
    }
    mWindow = NULL;
}


WindowSystem::WindowSystem()
    : mExitModel( VoidFunc( this, &WindowSystem::Close ), "game.exit", &RootModel::Get() )
    , mExit( false )
    , mWindow( NULL )
{

}

WindowSystem::~WindowSystem()
{
    Destroy();
}

void WindowSystem::Resize( const uint32_t Width, const uint32_t Height )
{
    if( !mWindow )
    {
        return;
    }
    glfwSetWindowSize( mWindow, Width, Height );
}

void WindowSystem::GetWindowSize( int& Width, int& Height ) const
{
    if( mWindow )
    {
        glfwGetFramebufferSize( mWindow, &Width, &Height );
    }
    else
    {
        Width = Height = 0;
    }
}

void WindowSystem::Close()
{
    mExit = true;
}

void WindowSystem::Init()
{

}

void WindowSystem::Update( double DeltaTime )
{
    if( !mWindow || glfwWindowShouldClose( mWindow ) || mExit )
    {
        EventServer<PhaseChangedEvent>& PhaseChangeEventServer( EventServer<PhaseChangedEvent>::Get() );
        PhaseChangeEventServer.SendEvent( PhaseChangedEvent( ProgramPhase::InitiateShutDown ) );
        return;
    }

    glfwSwapBuffers( mWindow );
    glfwPollEvents();
}

} // namespace engine
