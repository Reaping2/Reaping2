#include "platform/i_platform.h"
#include "engine/collisions/wall_collision_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/wall_collision_component.h"
#include "core/i_position_component.h"

namespace engine {

WallCollisionSubSystem::WallCollisionSubSystem()
    : CollisionSubSystem()
{

}

void WallCollisionSubSystem::Init()
{
}

void WallCollisionSubSystem::Update(Actor& actor, double DeltaTime)
{
    if (!mOther)
    {
        return;
    }

    Opt<IPositionComponent> otherPositionC = mOther->Get<IPositionComponent>();
    Opt<ICollisionComponent> otherCC = mOther->Get<ICollisionComponent>();
    Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
    Opt<WallCollisionComponent> wallCC = actor.Get<WallCollisionComponent>();
    if (!otherPositionC.IsValid()||!otherCC.IsValid()||!positionC.IsValid()||!wallCC.IsValid())
    {
        return;
    }

    double const dx = otherPositionC->GetX() - positionC->GetX();
    double const dy = otherPositionC->GetY() - positionC->GetY();
    double const r = wallCC->GetRadius() + otherCC->GetRadius();
    static const double Epsilon = std::numeric_limits<float>::epsilon() * 100;
    if( std::abs( dx ) > std::abs( dy ) )
    {
        if( 0 < dx && dx < r )
        {
            otherPositionC->SetX( otherPositionC->GetX() - dx + r + Epsilon );
        }
        else if( -r < dx && dx < 0 )
        {
            otherPositionC->SetX( otherPositionC->GetX() - r - Epsilon - dx );
        }
    }
    else if( std::abs( dx ) < std::abs( dy ) )
    {
        if( 0 < dy && dy < r )
        {
            otherPositionC->SetY( otherPositionC->GetY() - dy + r + Epsilon );
        }
        else if( -r < dy && dy < 0 )
        {
            otherPositionC->SetY( otherPositionC->GetY() - r - Epsilon - dy );
        }
    }

}
} // namespace engine

