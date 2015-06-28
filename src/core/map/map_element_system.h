#ifndef INCLUDED_MAP_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_MAP_ELEMENT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "map_system.h"

namespace map {

class MapElementSystem : public engine::System
{
public:
    MapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
    Scene& mScene;
    Opt<MapSystem> mMapSystem;
};

} // namespace map

#endif//INCLUDED_MAP_MAP_ELEMENT_SYSTEM_H
