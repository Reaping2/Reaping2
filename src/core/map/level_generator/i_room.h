#ifndef INCLUDED_MAP_I_ROOM_H
#define INCLUDED_MAP_I_ROOM_H

#include "platform/i_platform.h"
#include "room_desc.h"
#include "core/scene.h"
#include "../map_element_factory.h"
#include "../map_element.h"
#include "../soldier_spawn_point_map_element.h"
#include "../map_system.h"
#include "i_property.h"

namespace map {

class IRoom 
{
public:
    IRoom( int32_t Id );
    virtual ~IRoom();
    virtual void Generate( RoomDesc& roomDesc, glm::vec2 pos, bool editor = false ) = 0;
    virtual RoomDesc const& GetRoomDesc()const;
    int32_t GetId()const;
    typedef std::vector<glm::vec2> NeighbourCells_t;
    NeighbourCells_t const& GetNeighbourCells()const;
    virtual void Init();
    typedef boost::ptr_vector<IProperty> Properties_t;
    Properties_t const& GetProperties() const;
    Properties_t& GetProperties();
    Opt<IProperty> GetProperty( int32_t propUID );
    void AddProperty( std::auto_ptr<IProperty> prop );


    // don't use
    void _SetRoomDesc( RoomDesc const& roomDesc );
protected:
    NeighbourCells_t mNeighbourCells;
    RoomDesc mRoomDesc;
    int32_t mId = -1;
    Scene& mScene;
    Properties_t mProperties;
    void PlaceSoldierSpawnPoint( RoomDesc &roomDesc, glm::vec2 pos );
    void PlaceLevelEndPoint( RoomDesc &roomDesc, glm::vec2 pos );
    void AddNeighbourCell( glm::vec2 pos );
    bool IsInBounds( glm::vec2 pos ) const;
};

} // namespace map

#endif//INCLUDED_MAP_I_ROOM_H

//command:  "classgenerator.exe" -g "component" -c "room" -n "map" -m "RoomDesc-roomDesc"
