#ifndef INCLUDED_ENGINE_ATTACH_STATE_CHANGED_EVENT_H
#define INCLUDED_ENGINE_ATTACH_STATE_CHANGED_EVENT_H

#include "platform/event.h"

namespace engine {

struct AttachStateChangedEvent : public platform::Event
{
    enum Type
    {
        Attached = 0,
        Detached
    };
    Type mType;
    int32_t mAttachedGUID;
    int32_t mActorGUID;
    AttachStateChangedEvent( Type type, int32_t attachedGUID, int32_t actorGUID )
        : mType( type ), mAttachedGUID( attachedGUID ), mActorGUID( actorGUID ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_ATTACH_STATE_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "attach_state_changed_event" -m "int32_t-actorGUID int32_t-attachedGUID Type-type"
