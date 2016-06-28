#ifndef INCLUDED_MAP_I_ROOM_H
#define INCLUDED_MAP_I_ROOM_H

#include "platform/i_platform.h"
#include "room_desc.h"
#include "../../scene.h"
#include "i_level_generator.h"
#include "../map_element_factory.h"
#include "../map_element.h"
#include "../soldier_spawn_point_map_element.h"
#include "../map_system.h"

namespace map {

class IRoom 
{
public:
    IRoom( int32_t Id );
    virtual ~IRoom();
    virtual void Generate(RoomDesc& roomDesc, int32_t x, int32_t y)=0;
    virtual RoomDesc const& GetRoomDesc()const;
    int32_t GetId()const;
    typedef std::vector<glm::vec2> Neighbours_t;
    Neighbours_t const& GetNeighbours()const;
    virtual void Init();
protected:
    Neighbours_t mNeighbours;
    RoomDesc mRoomDesc;
    int32_t mId = -1;
    Scene& mScene;
    void PlaceSoldierSpawnPoint( RoomDesc &roomDesc, int32_t x, int32_t y );
    void PlaceLevelEndPoint( RoomDesc &roomDesc, int32_t x, int32_t y );
    void AddNeighbour( int32_t x, int32_t y );
    bool IsInBounds( glm::vec2 pos ) const;
};

} // namespace map

#endif//INCLUDED_MAP_I_ROOM_H

//command:  "classgenerator.exe" -g "component" -c "room" -n "map" -m "RoomDesc-roomDesc"
