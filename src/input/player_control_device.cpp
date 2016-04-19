#include "player_control_device.h"
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <vector>

namespace input {

PlayerControlDevice::PlayerControlDevice()
{
}

PlayerControlDevice::DeviceDesc::DeviceDesc()
    : deviceType( PlayerControlDevice::KeyboardAndMouse )
    , deviceId( -1 )
{
}

PlayerControlDevice::DeviceType PlayerControlDevice::GetControlDevice( int32_t controlledLocalPlayerId )
{
    return mControlMap[ controlledLocalPlayerId ].deviceType;
}

int32_t PlayerControlDevice::GetControllerIndex( int32_t controlledLocalPlayerId )
{
    return mControlMap[ controlledLocalPlayerId ].deviceId;
}

void PlayerControlDevice::SetControlDevice( int32_t controlledLocalPlayerId, std::string const& device )
{
    DeviceDesc& d = mControlMap[ controlledLocalPlayerId ];
    size_t sep = device.find_last_of( ':' );
    d.deviceType = device.substr( 0, sep ) == "controller" ? Controller : KeyboardAndMouse;
    if( sep != std::string::npos && d.deviceType == Controller )
    {
        d.deviceId = atoi( device.substr( sep + 1 ).c_str() );
    }
}

void PlayerControlDevice::SetControlDeviceConfiguration( std::string const& str )
{
    std::vector<std::string> strs;
    boost::split( strs, str, boost::is_any_of(",") );
    for( auto const& st : strs )
    {
        size_t sep = st.find_first_of( ':' );
        if( std::string::npos == sep )
        {
            continue;
        }
        static size_t const len = std::string( "player" ).size();
        if( sep < len )
        {
            continue;
        }
        int playerId = atoi( st.substr( len, sep - len ).c_str() );
        if( playerId == 0 )
        {
            continue;
        }
        SetControlDevice( playerId, st.substr( sep + 1 ) );
    }
}


}
