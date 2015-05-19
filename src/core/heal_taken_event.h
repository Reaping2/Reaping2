#ifndef INCLUDED_CORE_HEAL_TAKEN_EVENT_H
#define INCLUDED_CORE_HEAL_TAKEN_EVENT_H

#include "platform/event.h"

namespace core {

struct HealTakenEvent : public platform::Event
{
    double mX;
    double mY;
    int32_t mHeal;
    int32_t mActorGUID;
    HealTakenEvent(double x, double y, int32_t heal, int32_t actorGUID)
        :mX(x),mY(y),mHeal(heal),mActorGUID(actorGUID){}
};

} // namespace core

#endif//INCLUDED_CORE_HEAL_TAKEN_EVENT_H
