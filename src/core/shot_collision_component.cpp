#include "core/shot_collision_component.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/damage_action.h"
#include "core/i_health_component.h"
#include "core/component_factory.h"
#include "core/i_collision_component.h"
#include "core/scene.h"
#include "platform/i_platform.h"
#include "core/action_repo.h"

ShotCollisionComponent::ShotCollisionComponent()
    : CollisionComponent()
    , mDamage( 0 )
    , mParentGuid( -1 )
{

}

void ShotCollisionComponent::Collide(Actor& Other)
{
    if( Other.GetGUID() == mParentGuid )
    {
        return;
    }
    if (!mActor)
    {
        return;
    }

    //TODO: ofc not this way, factory should give me the right action type
    std::auto_ptr<Action> act = ActionRepo::Get()( AutoId("damage") );
    DamageAction* dact = static_cast<DamageAction*>(act.release());
    dact->SetDamage(GetDamage());
    Other.AddAction(std::auto_ptr<Action>(static_cast<Action*>(dact)));
    //TODO: action
    mActor->Get<IHealthComponent>()->SetHP(0);
}

void ShotCollisionComponent::ClipScene()
{
    if (!mActor)
    {
        return;
    }
    glm::vec4 const& AllowedDimensions = Scene::Get().GetDimensions();
    Opt<IPositionComponent> PositionC = mActor->Get<IPositionComponent>();    
    if( PositionC->GetX() + GetRadius() < AllowedDimensions.x * 2 ||
        PositionC->GetX() - GetRadius() > AllowedDimensions.z * 2 ||
        PositionC->GetY() + GetRadius() < AllowedDimensions.y * 2 ||
        PositionC->GetY() - GetRadius() > AllowedDimensions.w * 2 )
    {
        //TODO: action
        mActor->Get<IHealthComponent>()->SetHP(0);  
    }
}

void ShotCollisionComponent::SetDamage(int32_t Damage)
{
    mDamage=Damage;
}

int32_t ShotCollisionComponent::GetDamage() const
{
    return mDamage;
}

void ShotCollisionComponent::SetParent( Actor* Parent )
{
    mParentGuid = Parent->GetGUID();
}

void ShotCollisionComponentLoader::BindValues()
{
    Bind("damage",func_int32_t(&ShotCollisionComponent::SetDamage));
}

ShotCollisionComponentLoader::ShotCollisionComponentLoader()
{
    SetBase<CollisionComponentLoader>();
}
