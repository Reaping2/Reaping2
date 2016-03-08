#include "window.h"

namespace {
void Window_FramebufferSizeCallback( GLFWwindow* Window, int Width, int Height )
{
    EventServer<WindowResizeEvent>::Get().SendEvent( WindowResizeEvent( Width, Height ) );
}
}
namespace engine {
bool WindowSystem::Create( const uint32_t Width, const uint32_t Height, const std::string& Title )
{
    if( !glfwInit() )
    {
        LOG( "glfwInit failed!\n" );
        return false;
    }
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    mWindow = glfwCreateWindow( Width, Height, Title.c_str(), NULL, NULL );
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
    : mWindow( NULL )
    , mExitModel( VoidFunc( this, &WindowSystem::Close ), "game.exit", &RootModel::Get() )
    , mExit( false )
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
