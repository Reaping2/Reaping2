#include "i_core.h"

void Wall::TakeDamage( int32_t Damage )
{

}

void Wall::Collide( Actor& Other )
{
	Opt<PositionComponent> otherPositionC = Other.Get<PositionComponent>();
	Opt<PositionComponent> positionC = Get<PositionComponent>();

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

Wall::Wall( std::string const& Name )
    : Actor( Name )
{
    mFields[COLLISION_CLASS].i = CollisionClass::Wall;
    mFields[RADIUS].d = 0.1;
}
