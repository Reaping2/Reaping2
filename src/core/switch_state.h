#ifndef INCLUDED__SWITCH_STATE_H
#define INCLUDED__SWITCH_STATE_H

#include "platform/singleton.h"
#include "boost/bimap.hpp"

class SwitchState : public platform::Singleton<SwitchState>
{
protected:
    friend class platform::Singleton<SwitchState>;
    SwitchState();
public:
    enum Type
    {
        Off=0,
        TransitionToOn,
        On,
        TransitionToOff,
        Num_Classes
    };
    SwitchState::Type operator()( int32_t Id ) const;
    int32_t operator()( SwitchState::Type type ) const;
private:
    typedef boost::bimap<int32_t,SwitchState::Type> IdToSwitchStateMap_t;
    IdToSwitchStateMap_t mIdToSwitchStateMap;
};

#endif//INCLUDED__SWITCH_STATE_H


//command:  "classgenerator.exe" -g "enum" -c "switch_state" -m "Off-off Transition-transition On-on"
