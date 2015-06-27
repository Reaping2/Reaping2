#ifndef INCLUDED_MAP_LINK_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_LINK_MAP_ELEMENT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/map_loaded_event.h"
#include "map_system.h"

namespace map {

class LinkMapElementSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(LinkMapElementSystem)
    LinkMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    Opt<MapSystem> mMapSystem;
    AutoReg mOnMapLoaded;
    void OnMapLoaded(core::MapLoadedEvent const& Evt);
};

} // namespace map

#endif//INCLUDED_MAP_LINK_MAP_ELEMENT_SYSTEM_H
