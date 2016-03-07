#ifndef INCLUDED_MAP_SOLDIER_SPAWN_POINT_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_SOLDIER_SPAWN_POINT_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"
#include "core/map_start_event.h"

namespace map {

struct SpawnPoint
{
    int32_t mX;
    int32_t mY;
    SpawnPoint( int32_t x, int32_t y )
        : mX( x ), mY( y ) {}
};

typedef std::vector<SpawnPoint> SpawnPoints_t;

class SoldierSpawnPointMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE( SoldierSpawnPointMapElementSystem )
    SoldierSpawnPointMapElementSystem();
    SpawnPoints_t GetActiveSpawnPoints();
    static Opt<SoldierSpawnPointMapElementSystem> Get();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
};

} // namespace map

#endif//INCLUDED_MAP_SOLDIER_SPAWN_POINT_MAP_ELEMENT_SYSTEM_H

