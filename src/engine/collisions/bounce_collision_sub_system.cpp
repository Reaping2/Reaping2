#include "platform/i_platform.h"
#include "engine/engine.h"
#include "engine/collisions/bounce_collision_sub_system.h"
#include "core/bounce_collision_component.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/wall_collision_component.h"

namespace engine {

BounceCollisionSubSystem::BounceCollisionSubSystem()
    : CollisionSubSystem()
{
}


void BounceCollisionSubSystem::Init()
{
}


void BounceCollisionSubSystem::Update(Actor& actor, double DeltaTime)
{
    if (!mOther)
    {
        return;
    }
    //TODO: for now its wallcc should be a bouncableComponent or this should be a collision class 
    Opt<WallCollisionComponent> wallCC = mOther->Get<WallCollisionComponent>();
    if(!wallCC.IsValid())
    {
        return;
    }
    Opt<BounceCollisionComponent> bounceCC=actor.Get<BounceCollisionComponent>();
    Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();

    Opt<IPositionComponent> otherPositionC = mOther->Get<IPositionComponent>();
    Opt<ICollisionComponent> otherCC = mOther->Get<ICollisionComponent>();
    if (!otherPositionC.IsValid()||!otherCC.IsValid()||!positionC.IsValid())
    {
        return;
    }
    double const dx = otherPositionC->GetX() - positionC->GetX();
    double const dy = otherPositionC->GetY() - positionC->GetY();

    const double h = moveC->GetHeading();
    double c = cos( h );
    double s = sin( h );
    double at = atan2(s,c);
    double at2 = atan2(c,s);
    if( std::abs( dx ) > std::abs( dy ) )
    {
        c*=-1;
    }
    else if( std::abs( dx ) < std::abs( dy ) )
    {
        s*=-1;
    }
    moveC->SetHeading(atan2(s,c));
    moveC->GetSpeed().mBase.Set(moveC->GetSpeed().mBase.Get()*(1.0-bounceCC->GetSpeedLossPercent()));
}


void BounceCollisionSubSystem::ClipScene(Actor& actor)
{
    Opt<BounceCollisionComponent> bounceCC=actor.Get<BounceCollisionComponent>();
    Opt<ICollisionComponent> collisionC=actor.Get<ICollisionComponent>();
    Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    const double h = moveC->GetHeading();
    double c = cos( h );
    double s = sin( h );

    glm::vec4 AllowedDimensions = mScene.GetDimensions();
    float Radius = ( float )collisionC->GetRadius();
    AllowedDimensions.x += Radius;
    AllowedDimensions.y += Radius;
    AllowedDimensions.z -= Radius;
    AllowedDimensions.w -= Radius;
    bool bounced=false;
    if( positionC->GetX() < AllowedDimensions.x || positionC->GetX() > AllowedDimensions.z )
    {
        c*=-1;
        bounced=true;
    }
    if( positionC->GetY() < AllowedDimensions.y || positionC->GetY() > AllowedDimensions.w )
    {
        s*=-1;
        bounced=true;
    }
    if (bounced)
    {
        moveC->SetHeading(atan2(s,c));
        moveC->GetSpeed().mBase.Set(moveC->GetSpeed().mBase.Get()*(1.0-bounceCC->GetSpeedLossPercent()));
    }
    CollisionSubSystem::ClipScene(actor);
}


} // namespace engine

