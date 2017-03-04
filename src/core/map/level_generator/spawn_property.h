#ifndef INCLUDED_MAP_SPAWN_PROPERTY_H
#define INCLUDED_MAP_SPAWN_PROPERTY_H

#include "core/map/level_generator/i_property.h"
#include "platform/i_platform.h"
#include "entrance_type.h"
#include "../spawn_actor_map_element.h"
#include "../respawn_actor_map_element.h"

namespace map {

class SpawnProperty : public IProperty
{
public:
    DEFINE_ROOM_PROPERTY_BASE(SpawnProperty)
    SpawnProperty( int32_t Id );
    virtual void Load( Json::Value const& setters );
    virtual void Save( Json::Value& setters ) const;
    typedef std::vector<int32_t> Targets_t;
    void SetTargets( Targets_t blockedTargets );
    Targets_t const& GetTargets() const;
    void SetChance( int32_t chance );
    int32_t GetChance() const;
    virtual void Generate( RoomDesc& roomDesc, MapElementHolder& mMapElementHolder, glm::vec2 pos, bool editor = false );
    static void SpawnTargets( RoomDesc &roomDesc, std::vector<int32_t> targets, MapElementHolder &mMapElementHolder, glm::vec2 &pos );

    static void SpawnActor( Opt<SpawnActorMapElement> spawnActorMapElement, glm::vec2 &pos );
    static void RespawnActor( Opt<RespawnActorMapElement> respawnActorMapElement, glm::vec2 & pos );
private:
    Targets_t mTargets;
    int32_t mChance;
};


} // namespace map

#endif//INCLUDED_MAP_SPAWN_PROPERTY_H

//command:  "classgenerator.exe" -g "map_element" -c "cell_entrance_property" -p "property" -n "map" -m "int32_t-x int32_t-y EntranceType::Type-type Targets_t-blockedTargets Targets_t-entranceTargets"
