#include "platform/i_platform.h"
#include "platform/settings.h"
#include "controller_adapter_system.h"
#include "input_system.h"
#include "mapping.h"
#include "player_control_device.h"
#include "core/player_controller_component.h"
#include "core/actor.h"
#include "core/i_position_component.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <GLFW/glfw3.h>
#include <boost/predef.h>

namespace engine {
namespace {
#if BOOST_OS_WINDOWS
static std::string const platform = ".windows";
#elif BOOST_OS_LINUX
static std::string const platform = ".linux";
#elif BOOST_OS_MACOS
static std::string const platform = ".mac";
#else
static std::string const platform = ".unknown";
#endif
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
        , scheme( input::Mapping::Get().getMapping( "controller." + name + platform ) )
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
    , mCalibrate( false )
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

    // collect controlled players by controller id
    std::map<int32_t, Opt<Actor> > controlledPlayers;
    static core::ProgramState& ps =  core::ProgramState::Get();
    Opt<Actor> actor( mScene.GetActor( ps.mControlledActorGUID ) );
    Opt<core::ClientData> clientData( ps.FindClientDataByActorGUID( ps.mControlledActorGUID ) );
    if( clientData.IsValid() && actor.IsValid() )
    {
        controlledPlayers[ clientData->mControlledLocalPlayerId ] = actor;
    }

    for( auto joy : getJoysticks() )
    {
        // get keymapping for joy name @ joy pos ( if custom, support per-slot mapping )
        ControllerState cs( joy );
        if( !cs.isValid() )
        {
            continue;
        }

        if( mCalibrate )
        {
            std::cout << "Joy " << joy << "\n";
            std::cout << "\tname: '" << cs.name << "'\n";
            std::cout << "\taxes: \n";
            int cnt = 0;
            for( auto const& a : cs.axes )
            {
                std::cout << "\t\ta" << cnt++ << " " << a << "\n";
            }
            std::cout << "\tbuttons: \n";
            cnt = 0;
            for( auto const& b : cs.buttons )
            {
                std::cout << "\t\tb" << cnt++ << " " << b << "\n";
            }
        }

        // get controlled player for joy
        int32_t controlledLocalPlayerId = 1;
        static input::PlayerControlDevice& pcd( input::PlayerControlDevice::Get() );
        if( pcd.GetControlDevice( controlledLocalPlayerId ) != input::PlayerControlDevice::Controller ||
            pcd.GetControllerIndex( controlledLocalPlayerId ) != joy )
        {
            continue;
        }

        // get InputState for player
        InputState is = inputsys->GetInputState( controlledLocalPlayerId );

        // fill inputstate for player
        is.mShoot = cs.getBool( "shoot" );
        is.mShootAlt = cs.getBool( "shoot_alt" );
        is.mUseNormalItem = cs.getBool( "use_normal" );
        is.mActivate = cs.getBool( "activate" );
        is.mSwitchWeapon = cs.getBool( "switch_weapon" );
        is.mSwitchNormalItem = cs.getBool( "switch_item" );
        is.mReload = cs.getBool( "reload" );
        is.mShowLeaderboard = cs.getBool( "show_leaderboard" );
        is.mPause = cs.getBool( "pause" );
        double vh = cs.getDouble( "view_horizontal" ), vv = cs.getDouble( "view_vertical" );
        static Settings& settings( Settings::Get() );
        static double const viewNullZone = settings.GetDouble( "controllers.view_null_zone", 0.4 );
        if( std::abs( vv ) >= viewNullZone || std::abs( vh ) >= viewNullZone )
        {
            is.mOrientation = atan2( vv, vh );
        }
        Opt<Actor> actor = controlledPlayers[ controlledLocalPlayerId ];
        if( actor.IsValid() )
        {
            Opt<IPositionComponent> actorPositionC = actor->Get<IPositionComponent>();
            static double const radius = settings.GetDouble( "controllers.cursor_radius", 400 );
            is.mCursorX = actorPositionC->GetX() + radius * cos( is.mOrientation );
            is.mCursorY = actorPositionC->GetY() + radius * sin( is.mOrientation );
        }
        double mh = cs.getDouble( "move_horizontal" ), mv = cs.getDouble( "move_vertical" );
        is.mHeading = atan2( mv, mh );
        static double const moveNullZone = settings.GetDouble( "controllers.move_null_zone", 0.2 );
        is.mMoving = std::abs( mv ) >= moveNullZone || std::abs( mh ) >= moveNullZone;
        inputsys->SetInputState( controlledLocalPlayerId, is );
        // TODO fill generic input state ( ui )
    }
}

void ControllerAdapterSystem::SetCalibrate( bool set )
{
    mCalibrate = set;
}


} // namespace engine

