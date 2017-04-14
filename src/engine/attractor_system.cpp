#include "platform/i_platform.h"
#include "attractor_system.h"
#include "core/i_attractor_component.h"
#include "engine.h"
#include "collision_system.h"
#include "core/i_position_component.h"
#include "core/i_attractable_component.h"
#include "core/i_acceleration_component.h"
#include "core/i_target_holder_component.h"

namespace engine {

AttractorSystem::AttractorSystem()
    : mScene( Scene::Get() )
{
}


void AttractorSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IAttractorComponent>().IsValid()
            && actor.Get<IPositionComponent>().IsValid(); } );
}


void AttractorSystem::Update(double DeltaTime)
{
    for ( auto actor : mScene.GetActorsFromMap( GetType_static() ) )
    {
        auto attractorC = actor->Get<IAttractorComponent>();
        auto positionC = actor->Get<IPositionComponent>();
        if (!attractorC.IsValid()||!positionC.IsValid())
        {
            continue;
        }
        glm::vec2 const posVec2( positionC->GetX(), positionC->GetY() );
        static auto collisionSystem( ::engine::Engine::Get().GetSystem<engine::CollisionSystem>() );

        for (auto&& attractorDesc : attractorC->GetAttractorDescs())
        {
            auto&& collActors( collisionSystem->GetAllCollidingActors( posVec2, attractorDesc.mRange, attractorDesc.mCollisionClassMask ) );
            for (auto collActor : collActors)
            {
                auto attractableC( collActor->Get<IAttractableComponent>() );
                auto accelerationC( collActor->Get<IAccelerationComponent>() );
                if (attractableC.IsValid()&&accelerationC.IsValid()
                    && attractorDesc.mAttractTypes.find( attractableC->GetAttractType() ) != attractorDesc.mAttractTypes.end()
                    && attractorDesc.mAcceleration > accelerationC->GetAcceleration())
                {
                    auto targetHolderC( collActor->Get<ITargetHolderComponent>() );
                    if (targetHolderC.IsValid())
                    {
                        targetHolderC->SetTargetGUID( actor->GetGUID() );
                        accelerationC->SetAcceleration( attractorDesc.mAcceleration );
                    }
                }
            }
        }
    }
}


} // namespace engine

