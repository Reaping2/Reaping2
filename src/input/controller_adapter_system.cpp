#include "platform/i_platform.h"
#include "controller_adapter_system.h"
#include "input_system.h"
#include "mapping.h"
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
struct ControllerState
{
    std::string name;
    std::vector<double> axes;
    std::vector<bool> buttons;
    Json::Value const& scheme;
    ControllerState( int joy )
        : name( getName( joy ) )
        , axes( getAxes( joy ) )
        , buttons( getButtons( joy ) )
        , scheme( input::Mapping::Get().getMapping( "controller." + name ) )
    {
    }
    bool isValid()
    {
        return !!scheme;
    }
    bool getBool( std::string const& valname )
    {
        if( !isValid() )
        {
            return false;
        }
        Json::Value val = scheme[ valname ];
        char const* str = val.asCString();
        bool neg = '-' == *str;
        if( neg )
        {
            ++str;
        }
        int idx = 0;
        switch( *str++ )
        {
            case 'a':
                idx = atoi( str );
                return ( axes.size() > idx ? axes.at( idx ) >= 0 : false ) != neg;
            case 'b':
                idx = atoi( str );
                return ( buttons.size() > idx ? buttons.at( idx ) : false ) != neg;
        }
        return neg;
    }
    double getDouble( std::string const& valname )
    {
        if( !isValid() )
        {
            return 0.0;
        }
        Json::Value val = scheme[ valname ];
        char const* str = val.asCString();
        double neg = '-' == *str ? -1. : 1.;
        if( neg < 0 )
        {
            ++str;
        }
        int idx = 0;
        switch( *str++ )
        {
            case 'a':
                idx = atoi( str );
                return ( axes.size() > idx ? axes.at( idx ) : 0.0 ) * neg;
        }
        return neg;
    }
};
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
    Opt<InputSystem> inputsys = InputSystem::Get();
    if( !inputsys.IsValid() )
    {
        return;
    }

    for( auto joy : getJoysticks() )
    {
        if( joy != 2 )
        {
            continue;
        }
        // get controlled player for joy
        int32_t controlledLocalPlayerId = 1;

        // get InputState for player
        InputState is = inputsys->GetInputState( controlledLocalPlayerId );

        // get keymapping for joy name @ joy pos ( if custom, support per-slot mapping )
        ControllerState cs( joy );
        if( !cs.isValid() )
        {
            continue;
        }
        // fill inputstate for player
        is.mShoot = cs.getBool( "shoot" );
        is.mShootAlt = cs.getBool( "shoot_alt" );
        is.mUseNormalItem = cs.getBool( "use_normal" );
        is.mReload = cs.getBool( "reload" );
        is.mShowLeaderboard = cs.getBool( "show_leaderboard" );
        is.mPause = cs.getBool( "pause" );
        double vh = cs.getDouble( "view_horizontal" ), vv = cs.getDouble( "view_vertical" );
        if( std::abs( vv ) >= 0.2 || std::abs( vh ) >= 0.2 )
        {
            is.mOrientation = atan2( vv, vh );
        }
        double mh = cs.getDouble( "move_horizontal" ), mv = cs.getDouble( "move_vertical" );
        is.mHeading = atan2( mv, mh );
        is.mMoving = std::abs( mv ) >= 0.2 || std::abs( mh ) >= 0.2;
        inputsys->SetInputState( controlledLocalPlayerId, is );
        // fill generic input state ( ui )

        std::cout << "joy " << joy << " " << cs.name << "\n";
        for( auto a : cs.axes )
        {
            std::cout << "\ta " << a << "\n";
        }
        for( auto b : cs.buttons )
        {
            std::cout << "\tb " << b << "\n";
        }
    }
}


} // namespace engine

