#ifndef INCLUDED_MAP_I_ROOM_H
#define INCLUDED_MAP_I_ROOM_H

#include "platform/i_platform.h"
#include "room_desc.h"
#include "../../scene.h"
#include "i_level_generator.h"

namespace map {

class IRoom 
{
public:
    IRoom( int32_t Id );
    virtual ~IRoom();
    virtual void Generate(RoomDesc& roomDesc, int32_t x, int32_t y)=0;
    virtual bool PlaceRoom(ILevelGenerator& levelGenerator, int32_t x, int32_t y);
    virtual void InsertNeighbours( ILevelGenerator& levelGenerator, int32_t x, int32_t y );
    virtual RoomDesc GetRoomDesc()const;
    int32_t GetId()const;
protected:
    RoomDesc mRoomDesc;
    int32_t mId = -1;
    Scene& mScene;
    virtual void InsertNeighbour( ILevelGenerator& levelGenerator, int32_t x, int32_t y );
};

} // namespace map

#endif//INCLUDED_MAP_I_ROOM_H

//command:  "classgenerator.exe" -g "component" -c "room" -n "map" -m "RoomDesc-roomDesc"
