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
    virtual void Generate( RoomDesc& roomDesc, glm::vec2 pos );
    void Load( Json::Value& setters );
    typedef boost::ptr_vector<IProperty> Properties_t;
    Properties_t const& GetProperties() const;
private:
    ActorFactory& mActorFactory;
    MapElementFactory& mMapElementFactory;
    PropertyFactory& mPropertyFactory;
    MapElementHolder mMapElementHolder;
    void ClearMapElements();
    Properties_t mProperties;
};

} // namespace map

#endif//INCLUDED_MAP_JSON_ROOM_H
