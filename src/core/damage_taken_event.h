#ifndef INCLUDED_CORE_DAMAGE_TAKEN_EVENT_H
#define INCLUDED_CORE_DAMAGE_TAKEN_EVENT_H
#include "platform/i_platform.h"

namespace core {
struct DamageTakenEvent : public Event
{
    glm::vec2 const Pos;
    int32_t Damage;
    int32_t ActorGUID;
    DamageTakenEvent( double x, double y ): Pos( x, y ) {}
    DamageTakenEvent( glm::vec2 const& p ): Pos( p ) {}
};
}
#endif//INCLUDED_CORE_DAMAGE_TAKEN_EVENT_H