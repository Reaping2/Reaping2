#include "turn_to_target_act.h"
#include "engine/path_system.h"
#include "engine/engine.h"
#include "../i_target_holder_component.h"
#include "../i_position_component.h"
#include "../i_move_component.h"
#include "../../engine/collision_system.h"
#include "../i_collision_component.h"

namespace scriptedcontroller
{

void TurnToTargetAct::Update( Actor& actor, double Seconds )
{
    IAct::Update( actor, Seconds );
    static auto mPathSystem = engine::Engine::Get().GetSystem<engine::path::PathSystem>();
    auto targetHolderC( actor.Get<ITargetHolderComponent>() );
    if (!targetHolderC.IsValid())
    {
        return;
    }
    static auto& mScene = Scene::Get();
    auto target( mScene.GetActor( targetHolderC->GetTargetGUID() ) );
    if (!target.IsValid())
    {
        return;
    }
    auto positionC( actor.Get<IPositionComponent>() );
    if (!positionC.IsValid())
    {
        return;
    }

    auto targetPositionC( target->Get<IPositionComponent>() );
    if (!targetPositionC.IsValid())
    {
        return;
    }
    auto collisionC( actor.Get<ICollisionComponent>() );
    if (!collisionC.IsValid())
    {
        return;
    }
    glm::vec2 const distV( (targetPositionC->GetX() - positionC->GetX()), (targetPositionC->GetY() - positionC->GetY()) );
    static auto mCollisionSystem = engine::Engine::Get().GetSystem<engine::CollisionSystem>();
    Opt<Actor> firstCollActor;
    if (mSeekPath)
    {
        firstCollActor =
            mCollisionSystem->GetFirstCollidingActor(
                actor, distV,
                collisionC->GetRadius() / 2.0,
                1 << CollisionClass::Player | 1 << CollisionClass::Wall );
    }
    double direction = 0.0;
    if (!mSeekPath
        ||(firstCollActor.IsValid() && firstCollActor->GetGUID() == target->GetGUID()))
    {
        // clear vision on target!
        direction = atan2( distV.y, distV.x );
        L2( "clear vision direction!\n" );
    }
    else
    {
        direction = mPathSystem->GetDirection( actor, *target );
    }

    static double const pi = boost::math::constants::pi<double>();

    auto moveC( actor.Get<IMoveComponent>() );
    auto useOrientation = mUseOrientation||!moveC.IsValid();
    auto actorDir = useOrientation ? positionC->GetOrientation() : moveC->GetHeading();
    double rads = 0.0;
    if (mSpeed != 0.0) // if speed is 0.0 then you need to instantly turn
    {
        rads = direction - actorDir;
        rads = fmod( rads + pi, pi * 2 );
        rads += pi * (rads < 0 ? 1 : -1);
    }
    if (std::abs( rads ) > 0.1)
    {
        direction = actorDir + (rads > 0 ? 1 : -1)*mSpeed*Seconds;
    }
    if (useOrientation)
    {
        positionC->SetOrientation( direction );
    }
    else
    {
        moveC->SetHeading( direction );
    }
}

void TurnToTargetAct::Load( Json::Value const& setters )
{
    IAct::Load( setters );
    double speedDeg = 100;
    Json::GetDouble( setters["speed"], speedDeg );
    mSpeed = speedDeg / 180 * boost::math::constants::pi<double>();
    Json::GetBool( setters["seek_path"], mSeekPath );
    Json::GetBool( setters["use_orientation"], mUseOrientation );
}


void TurnToTargetAct::Start( Actor& actor )
{
    IAct::Start( actor );
}

void TurnToTargetAct::Stop( Actor& actor )
{
    IAct::Stop( actor );
}


} // namespace scriptedcontroller

