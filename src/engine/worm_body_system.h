#ifndef INCLUDED_ENGINE_WORM_BODY_SYSTEM_H
#define INCLUDED_ENGINE_WORM_BODY_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/actor_factory.h"
#include "core/i_worm_head_component.h"

namespace engine {

class WormBodySystem : public System
{
public:
    DEFINE_SYSTEM_BASE(WormBodySystem)
    WormBodySystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );

    void CreateNewHead( Opt<Actor> wormHead, Opt<IWormHeadComponent> wormHeadC, Opt<Actor> actor );

private:
    Scene& mScene;
    ActorFactory& mActorFactory = ActorFactory::Get();
};

} // namespace engine

#endif//INCLUDED_ENGINE_WORM_BODY_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "worm_body_component" -m "int32_t-headGUID bool-body int32_t-followedGUID PrevPositions_t-prevPositions"
