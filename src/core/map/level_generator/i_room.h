#ifndef INCLUDED_MAP_I_ROOM_H
#define INCLUDED_MAP_I_ROOM_H

#include "platform/i_platform.h"
#include "room_desc.h"
#include "../../scene.h"

namespace map {

class IRoom 
{
public:
    IRoom( int32_t Id );
    virtual ~IRoom();
    virtual void Generate(RoomDesc const& roomDesc)=0;
    virtual RoomDesc GetRoomDesc()const;
protected:
    RoomDesc mRoomDesc;
    int32_t mId = -1;
    Scene& mScene;
};

} // namespace map

#endif//INCLUDED_MAP_I_ROOM_H

//command:  "classgenerator.exe" -g "component" -c "room" -n "map" -m "RoomDesc-roomDesc"
