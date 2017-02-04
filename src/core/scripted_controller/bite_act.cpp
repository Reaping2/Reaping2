#include "bite_act.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"
#include "engine/collision_system.h"
#include "engine/engine.h"
#include "core/i_collision_component.h"
#include "core/i_health_component.h"
#include "core/scene.h"
#include "core/program_state.h"

namespace scriptedcontroller
{

void BiteAct::Update( Actor& actor, double Seconds )
{
    IAct::Update( actor, Seconds );
    if (!mTimer.IsTime())
    {
        mTimer.Update( Seconds );
        return;
    }
    static auto collisionSystem( ::engine::Engine::Get().GetSystem<engine::CollisionSystem>() );
    auto positionC(actor.Get<IPositionComponent>());
    if (!positionC.IsValid())
    {
        return;
    }
    auto collisionC( actor.Get<ICollisionComponent>() );
    if (!collisionC.IsValid())
    {
        return;
    }
    auto headPosition = glm::vec2( positionC->GetX(), positionC->GetY() );
    if (!mCenter)
    {
        headPosition = glm::vec2(
            positionC->GetX() + cos( positionC->GetOrientation() )*collisionC->GetRadius()
            , positionC->GetY() + sin( positionC->GetOrientation() )*collisionC->GetRadius() );
    }
    auto&& players(collisionSystem->GetAllCollidingActors( headPosition,mRadius,1<<CollisionClass::Player ));
    if (players.size() > 0)
    {
        mTimer.Reset();
    }
    for (auto&& player : players)
    {
        auto healthC( player->Get<IHealthComponent>() );
        if (healthC.IsValid())
        {
            healthC->TakeDamage( mDamage );
        }
    }
}

void BiteAct::Load( Json::Value const& setters )
{
    IAct::Load( setters );
    mTimer.Load( setters["frequency"] );

    Json::GetDouble( setters["damage"], mDamage );
    Json::GetDouble( setters["radius"], mRadius );
    Json::GetBool( setters["center"], mCenter );
}


void BiteAct::Start( Actor& actor )
{
    IAct::Start( actor );
    mTimer.Reset();
    mTimer.Update( mTimer.GetFrequency() );
}

void BiteAct::Stop( Actor& actor )
{
    IAct::Stop( actor );
}


} // namespace scriptedcontroller

