#ifndef INCLUDED_ENGINE_WORM_HEAD_SYSTEM_H
#define INCLUDED_ENGINE_WORM_HEAD_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/actor_factory.h"
class Actor;
class IWormHeadComponent;
class IPositionComponent;

namespace engine {

class WormHeadSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(WormHeadSystem)
    WormHeadSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );

    void SetBodyPartPositions( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC );

    void AddPrevPositions( Opt<IPositionComponent> positionC, Opt<IWormHeadComponent> wormHeadC );

private:
    Scene& mScene;
    ActorFactory& mActorFactory=ActorFactory::Get();
    void InitWormPart( Actor& head, Actor& part );
};

} // namespace engine

#endif//INCLUDED_ENGINE_WORM_HEAD_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "worm_head_component" -m "int32_t-length int32_t-lengthDecrease int32_t-bodyId int32_t-tailId double-spawnDelay double-spawnDelayMax int32_t-spawnCount bool-firstBuild BodyParts_t-bodyParts"
