#include "core/switch_state.h"
#include "platform/auto_id.h"

using platform::AutoId;

SwitchState::SwitchState()
{
    mIdToSwitchStateMap.insert(IdToSwitchStateMap_t::value_type(AutoId("off"),SwitchState::Off));
    mIdToSwitchStateMap.insert(IdToSwitchStateMap_t::value_type(AutoId("transition_to_on"),SwitchState::TransitionToOn));
    mIdToSwitchStateMap.insert(IdToSwitchStateMap_t::value_type(AutoId("on"),SwitchState::On));
    mIdToSwitchStateMap.insert( IdToSwitchStateMap_t::value_type( AutoId( "transition_to_off" ), SwitchState::TransitionToOff ) );
}

SwitchState::Type SwitchState::operator()( int32_t Id ) const
{
    IdToSwitchStateMap_t::left_const_iterator i=mIdToSwitchStateMap.left.find(Id);
    BOOST_ASSERT(i!=mIdToSwitchStateMap.left.end());
    return (i!=mIdToSwitchStateMap.left.end())?i->second:SwitchState::Off;
}

int32_t SwitchState::operator()( Type type ) const
{
    IdToSwitchStateMap_t::right_const_iterator i=mIdToSwitchStateMap.right.find(type);
    BOOST_ASSERT(i!=mIdToSwitchStateMap.right.end());
    return (i!=mIdToSwitchStateMap.right.end())?i->second:SwitchState::Off;
}

