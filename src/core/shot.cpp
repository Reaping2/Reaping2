#include "i_core.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"

Shot::Shot( std::string const& Name )
    : Actor( Name )
    , mParentGuid( -1 )
{
    mFields[COLLISION_CLASS].i = CollisionClass::Projectile;
    mFields[RADIUS].d = 0.02;
    mFields[DAMAGE].i = 10;
    Get<IMoveComponent>()->SetSpeed( 2 );
    AddAction( AutoId( "move" ) );
}

void Shot::ClipScene()
{
    glm::vec4 const& AllowedDimensions = Scene::Get().GetDimensions();
    Opt<IPositionComponent> PositionC = Get<IPositionComponent>();    
    if( PositionC->GetX() + GetRadius() < AllowedDimensions.x * 2 ||
        PositionC->GetX() - GetRadius() > AllowedDimensions.z * 2 ||
        PositionC->GetY() + GetRadius() < AllowedDimensions.y * 2 ||
        PositionC->GetY() - GetRadius() > AllowedDimensions.w * 2 )
    {
        mFields[HP].i = HP_DEAD;
    }
}

void Shot::Collide( Actor& Other )
{
    if( Other.GetGUID() == mParentGuid )
    {
        return;
    }
    Other.TakeDamage( mFields[DAMAGE].i );
    mFields[HP].i = HP_DEAD;
}

void Shot::UpdateLifetime()
{
    if( !IsAlive() )
    {
        delete this;
    }
}

void Shot::SetParent( Actor& P )
{
    mParentGuid = P.GetGUID();
}
