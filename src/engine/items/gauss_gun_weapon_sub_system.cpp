#include "engine/items/common_sub_system_includes.h"
#include "engine/items/gauss_gun_weapon_sub_system.h"
#include "core/gauss_gun.h"
#include "core/buffs/move_speed_buff.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"

namespace engine {

GaussGunWeaponSubSystem::GaussGunWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mShotId( AutoId( "gauss_shot" ) )
    , mAltShotId( AutoId( "gauss_alt_shot" ) )
{
}

void GaussGunWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void GaussGunWeaponSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<GaussGun> weapon = inventoryC->GetSelectedWeapon();
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if ( weapon->GetShootAlt() )
    {
        weapon->StartCharge();
    }
    else
    {
        weapon->EndCharge();
    }
    if ( weapon->GetCooldown() )
    {
        weapon->EndCharge();
    }
    if ( moveC.IsValid() && !moveC->IsRooted() && weapon->IsCharging() )
    {
        Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
        if(buffHolderC.IsValid())
        {
            std::auto_ptr<Buff> buff(core::BuffFactory::Get()(MoveSpeedBuff::GetType_static()));
            MoveSpeedBuff* moveSpeedBuff= (MoveSpeedBuff*)buff.get();
            moveSpeedBuff->SetRooted( true );
            moveSpeedBuff->SetFlatBonus( 0 );
            moveSpeedBuff->SetPercentBonus( 0.0 );
            moveSpeedBuff->SetAutoRemove( true );
            moveSpeedBuff->SetSecsToEnd( weapon->ChargeTime() );
            buffHolderC->AddBuff(buff);
        }
    }
    if (weapon->GetCooldown()>0)
    {
        return;
    }
    if (weapon->IsShooting())
    {
        EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),false);
    }
    else if (weapon->IsShootingAlt())
    {
        EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mAltShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),true);
    }
}

} // namespace engine

