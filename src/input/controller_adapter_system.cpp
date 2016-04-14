#include "platform/i_platform.h"
#include "controller_adapter_system.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <GLFW/glfw3.h>

namespace engine {
namespace {
std::vector<int> getJoysticks()
{
    std::vector<int> rv;
    for( int i = GLFW_JOYSTICK_1; i != GLFW_JOYSTICK_16; ++i )
    {
        if( glfwJoystickPresent( i ) )
        {
            rv.push_back( i );
        }
    }
    return rv;
}
std::vector<double> getAxes( int joy )
{
    int count = 0;
    const float* axes = glfwGetJoystickAxes( joy, &count );
    return std::vector<double>( axes, axes + count );
}
std::vector<bool> getButtons( int joy )
{
    int count;
    const unsigned char* axes = glfwGetJoystickButtons( joy, &count );
    std::vector<bool> rv;
    std::transform( axes, axes + count, std::back_inserter( rv ), []( char c ){ return c == GLFW_PRESS; } );
    return rv;
}
std::string getName( int joy )
{
    return glfwGetJoystickName( joy );
}
}

ControllerAdapterSystem::ControllerAdapterSystem()
    : mScene( Scene::Get() )
{
}


void ControllerAdapterSystem::Init()
{
}


void ControllerAdapterSystem::Update(double DeltaTime)
{
    for( auto joy : getJoysticks() )
    {
        // get controlled player for joy

        // get InputState for player

        auto const& name = getName( joy );
        // get keymapping for joy name @ joy pos ( if custom, support per-slot mapping )
        auto const& axes = getAxes( joy );
        auto const& buttons = getButtons( joy );
        // fill inputstate for player

        // fill generic input state ( ui )

        std::cout << "joy " << joy << " " << name << "\n";
        for( auto a : getAxes( joy ) )
        {
            std::cout << "\ta " << a << "\n";
        }
        for( auto b : getButtons( joy ) )
        {
            std::cout << "\tb " << b << "\n";
        }
    }
}


} // namespace engine

