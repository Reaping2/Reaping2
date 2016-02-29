#pragma once
#ifndef INCLUDED_CORPSES_H
#define INCLUDED_CORPSES_H

#include "platform/i_platform.h"
#include "core/actor_event.h"
#include "decal_engine.h"

class Corpses : public Singleton<Corpses>
{
    DecalEngine& mDecalEngine;
    Corpses();
    friend class Singleton<Corpses>;
    AutoReg mOnActorEvent;
    void OnActorEvent( ActorEvent const& Evt );
};


#endif // INCLUDED_CORPSES_H

