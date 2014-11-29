#include "core/i_position_component.h"
#include "core/i_collision_component.h"
#include "core/wall_collision_component.h"
#include "core/actor.h"
#include "platform/i_platform.h"


WallCollisionComponent::WallCollisionComponent()
    : CollisionComponent()
{
}

void WallCollisionComponent::Collide(Actor& Other)
{
    //TODO: kick_action or stglikethat
    Opt<IPositionComponent> otherPositionC = Other.Get<IPositionComponent>();
    Opt<IPositionComponent> positionC = mActor->Get<IPositionComponent>();

    double const dx = otherPositionC->GetX() - positionC->GetX();
    double const dy = otherPositionC->GetY() - positionC->GetY();
    double const r = GetRadius() + Other.Get<ICollisionComponent>()->GetRadius();
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
