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
    static void InitNewHead( Opt<Actor> newHead );
    static void ErasePrevPositions( Opt<IWormHeadComponent> wormHeadC, double eraseDistance );
    static double GetGapSize( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC );
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    ActorFactory& mActorFactory=ActorFactory::Get();

    void InitWormPart( Opt<Actor> head, Opt<Actor> part, Opt<IPositionComponent> positionC );

    void HandleFirstBuild( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC, Opt<IPositionComponent> positionC );
    void HandleDeath( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC, double DeltaTime );
    void HandleLengthIncrease( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC, Opt<IPositionComponent> positionC, double DeltaTime );

    void SyncMove( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC );
    void SetBodyPartPositions( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC );
    void AddPrevPositions( Opt<Actor> actor, Opt<IPositionComponent> positionC, Opt<IWormHeadComponent> wormHeadC );
};

} // namespace engine

#endif//INCLUDED_ENGINE_WORM_HEAD_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "worm_head_component" -m "int32_t-length int32_t-lengthDecrease int32_t-bodyId int32_t-tailId double-spawnDelay double-spawnDelayMax int32_t-spawnCount bool-firstBuild BodyParts_t-bodyParts"
