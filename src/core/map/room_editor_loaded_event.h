#ifndef INCLUDED_MAP_ROOM_EDITOR_LOADED_EVENT_H
#define INCLUDED_MAP_ROOM_EDITOR_LOADED_EVENT_H

#include "platform/event.h"

namespace map {

struct RoomEditorLoadedEvent : public platform::Event
{
    Opt<RoomDesc> mRoomDesc;
    RoomEditorLoadedEvent(Opt<RoomDesc> roomDesc)
        :mRoomDesc(roomDesc){}
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_EDITOR_LOADED_EVENT_H

//command:  "classgenerator.exe" -g "event" -n "map" -c "room_editor_loaded_event" -m "Opt<RoomDesc>-roomDesc"
