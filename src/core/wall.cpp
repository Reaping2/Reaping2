#include "i_core.h"
#include "core/i_position_component.h"
#include "core/collision_component.h"

Wall::Wall( std::string const& Name )
    : Actor( Name )
{
    AddComponent( mComponentFactory(AutoId("collision_component")));
    Opt<CollisionComponent> collisionC = Get<CollisionComponent>();
    collisionC->SetRadius(0.01);
    collisionC->SetCollisionClass(CollisionClass::Wall);

    mFields[COLLISION_CLASS].i = CollisionClass::Wall;
    mFields[RADIUS].d = 0.1;
}

void Wall::Collide( Actor& Other )
{
    Opt<IPositionComponent> otherPositionC = Other.Get<IPositionComponent>();
    Opt<IPositionComponent> positionC = Get<IPositionComponent>();

    double const dx = otherPositionC->GetX() - positionC->GetX();
    double const dy = otherPositionC->GetY() - positionC->GetY();
    double const r = GetRadius() + Other.GetRadius();
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
