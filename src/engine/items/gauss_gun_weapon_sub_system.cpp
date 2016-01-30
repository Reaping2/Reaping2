#include "engine/items/common_sub_system_includes.h"
#include "engine/items/gauss_gun_weapon_sub_system.h"
#include "core/gauss_gun.h"
#include "core/buffs/move_speed_buff.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"
#include "core/i_audible_component.h"

namespace engine {

GaussGunWeaponSubSystem::GaussGunWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mShotId( AutoId( "gauss_shot" ) )
    , mAltShotId( AutoId( "gauss_alt_shot" ) )
    , mProgramState( core::ProgramState::Get())
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
    if ( weapon->GetCooldown() > 0 || weapon->GetReloadTime() > 0 )
    {
        weapon->EndCharge();
    }
    if (mProgramState.mMode==core::ProgramState::Client)
    {
        return;
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
    Opt<IAudibleComponent> ac = actor.Get<IAudibleComponent>();
    if (weapon->IsShooting())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),false);
        if( ac.IsValid() )
        {
            ac->AddOneShotEffect( mShotId );
        }
    }
    else if (weapon->IsShootingAlt())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mAltShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),true);
        if( ac.IsValid() )
        {
            ac->AddOneShotEffect( mAltShotId );
        }
    }
}

} // namespace engine

