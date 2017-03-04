#ifndef INCLUDED_CORE_DAMAGE_TAKEN_EVENT_H
#define INCLUDED_CORE_DAMAGE_TAKEN_EVENT_H
#include "platform/i_platform.h"

namespace core {
struct DamageTakenEvent : public Event
{
    enum Type : int32_t
    {
        Health = 0,
        Armor
    };
    glm::vec2 const Pos;
    int32_t Damage = 0;
    Type type = Health;
    int32_t ActorGUID = -1;
    DamageTakenEvent( double x, double y ): DamageTakenEvent( glm::vec2( x, y ) ) {}
    DamageTakenEvent( glm::vec2 const& p ): Pos( p ) {}
};
}
#endif//INCLUDED_CORE_DAMAGE_TAKEN_EVENT_H
