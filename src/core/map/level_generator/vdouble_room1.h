#ifndef INCLUDED_MAP_VDOUBLE_ROOM1_H
#define INCLUDED_MAP_VDOUBLE_ROOM1_H

#include "i_room.h"
#include "core/actor_factory.h"

namespace map {

class VDoubleRoom1 : public IRoom
{
public:
    VDoubleRoom1( int32_t Id );
    virtual void Generate( RoomDesc& roomDesc, int32_t x, int32_t y );
private:
    ActorFactory& mActorFactory;
};

} // namespace map

#endif//INCLUDED_MAP_VDOUBLE_ROOM1_H
