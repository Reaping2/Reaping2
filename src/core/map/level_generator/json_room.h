#ifndef INCLUDED_MAP_JSON_ROOM_H
#define INCLUDED_MAP_JSON_ROOM_H

#include "i_room.h"
#include "core/actor_factory.h"

namespace map {

class JsonRoom : public IRoom
{
public:
    JsonRoom( int32_t Id );
    virtual void Generate( RoomDesc& roomDesc, glm::vec2 pos );
    void Load( Json::Value& setters );
private:
    ActorFactory& mActorFactory;
    MapElementFactory& mMapElementFactory;
    MapElementHolder mMapElementHolder;
    void ClearMapElements();
};

} // namespace map

#endif//INCLUDED_MAP_JSON_ROOM_H
