#ifndef INCLUDED_MAP_JSON_ROOM_H
#define INCLUDED_MAP_JSON_ROOM_H

#include "i_room.h"
#include "core/actor_factory.h"
#include "property_factory.h"

namespace map {

class JsonRoom : public IRoom
{
public:
    JsonRoom( int32_t Id );
    virtual void Generate( RoomDesc& roomDesc, glm::vec2 pos, bool editor = false );
    void Load( Json::Value const& setters );
    void Save( Json::Value& setters, RoomDesc const& roomDesc );
private:
    ActorFactory& mActorFactory;
    MapElementFactory& mMapElementFactory;
    PropertyFactory& mPropertyFactory;
    MapElementHolder mMapElementHolder;
    void ClearMapElements();
};

} // namespace map

#endif//INCLUDED_MAP_JSON_ROOM_H
