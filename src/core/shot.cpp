#include "i_core.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/damage_action.h"
#include "core/i_health_component.h"
#include "core/component_factory.h"


Shot::Shot( std::string const& Name )
    : Actor( Name )
    , mParentGuid( -1 )
{
    mFields[COLLISION_CLASS].i = CollisionClass::Projectile;
    mFields[RADIUS].d = 0.02;
    mFields[DAMAGE].i = 10;
    Get<IMoveComponent>()->SetSpeed( 2 );
    AddAction( AutoId( "move" ) );
    AddComponent(ComponentFactory::Get()(AutoId("health_delete_component")));
    Get<IHealthComponent>()->SetActor(this);
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
        //TODO: action
        Get<IHealthComponent>()->SetHP(0);  
    }
}

void Shot::Collide( Actor& Other )
{
    if( Other.GetGUID() == mParentGuid )
    {
        return;
    }

    //TODO: ofc not this way, factory should give me the right action type
    std::auto_ptr<Action> act = ActionRepo::Get()( AutoId("damage") );
    DamageAction* dact = static_cast<DamageAction*>(act.release());
    dact->SetDamage(mFields[DAMAGE].i);
    Other.AddAction(std::auto_ptr<Action>(static_cast<Action*>(dact)));
    //TODO: action

    Get<IHealthComponent>()->SetHP(0);
}

void Shot::SetParent( Actor& P )
{
    mParentGuid = P.GetGUID();
}
