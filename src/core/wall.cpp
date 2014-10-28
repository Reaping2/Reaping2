#include "i_core.h"

void Wall::TakeDamage( int32_t Damage )
{

}

void Wall::Collide( Actor& Other )
{
    double const dx = Other.GetX() - GetX();
    double const dy = Other.GetY() - GetY();
    double const r = GetRadius() + Other.GetRadius();
    static const double Epsilon = std::numeric_limits<float>::epsilon() * 100;
    if( std::abs( dx ) > std::abs( dy ) )
    {
        if( 0 < dx && dx < r )
        {
            Other.SetX( Other.GetX() - dx + r + Epsilon );
        }
        else if( -r < dx && dx < 0 )
        {
            Other.SetX( Other.GetX() - r - Epsilon - dx );
        }
    }
    else if( std::abs( dx ) < std::abs( dy ) )
    {
        if( 0 < dy && dy < r )
        {
            Other.SetY( Other.GetY() - dy + r + Epsilon );
        }
        else if( -r < dy && dy < 0 )
        {
            Other.SetY( Other.GetY() - r - Epsilon - dy );
        }
    }
}

Wall::Wall( std::string const& Name )
    : Actor( Name )
{
    mFields[COLLISION_CLASS].i = CollisionClass::Wall;
    mFields[RADIUS].d = 0.1;
}
