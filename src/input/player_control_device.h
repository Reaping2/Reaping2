#pragma once
#ifndef INCLUDED_PLAYER_CONTROL_DEVICE_H
#define INCLUDED_PLAYER_CONTROL_DEVICE_H

#include "platform/singleton.h"
#include "platform/rstdint.h"
#include <map>
#include <string>

namespace input {
class PlayerControlDevice : public platform::Singleton<PlayerControlDevice>
{
public:
    enum DeviceType {
        KeyboardAndMouse,
        Controller,
    };
    DeviceType GetControlDevice( int32_t controlledLocalPlayerId );
    int32_t GetControllerIndex( int32_t controlledLocalPlayerId );
    void SetControlDeviceConfiguration( std::string const& str );
private:
    void SetControlDevice( int32_t controlledLocalPlayerId, std::string const& device );
    friend class platform::Singleton<PlayerControlDevice>;
    PlayerControlDevice();
    struct DeviceDesc
    {
        DeviceDesc();
        DeviceType deviceType;
        int32_t deviceId;
    };
    typedef std::map<int32_t, DeviceDesc> ControlMap;
    ControlMap mControlMap;
};
}

#endif // INCLUDED_PLAYER_CONTROL_DEVICE_H

