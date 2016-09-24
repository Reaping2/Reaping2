#ifndef INCLUDED_MAP_SIMPLE_ROOM1_H
#define INCLUDED_MAP_SIMPLE_ROOM1_H

#include "i_room.h"
#include "core/actor_factory.h"

namespace map {

class SimpleRoom1 : public IRoom
{
public:
    SimpleRoom1( int32_t Id );
    virtual void Generate( RoomDesc& roomDesc, glm::vec2 pos, bool editor = false );
private:
    ActorFactory& mActorFactory;
};

} // namespace map

#endif//INCLUDED_MAP_SIMPLE_ROOM1_H
