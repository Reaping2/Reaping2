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
    , mWeaponItemSubSystem( WeaponItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
    , mProgramState( core::ProgramState::Get() )
{
}

void GaussGunWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void GaussGunWeaponSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<GaussGun> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if ( weapon->GetShootAlt() && weapon->GetCooldown() <= 0.0 )
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
    Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
    if ( buffHolderC.IsValid() && weapon->IsCharging() )
    {
        bool needsNew = true;
        BuffListFilter<IBuffHolderComponent::All> buffListFilter( buffHolderC->GetBuffList(), MoveSpeedBuff::GetType_static() );
        for ( BuffListFilter<IBuffHolderComponent::All>::const_iterator moveSpeedBuffIt = buffListFilter.begin(), moveSpeedBuffE = buffListFilter.end(); needsNew && moveSpeedBuffIt != moveSpeedBuffE; ++moveSpeedBuffIt )
        {
            Opt<MoveSpeedBuff> moveSpeedBuff( *moveSpeedBuffIt );
            needsNew = !moveSpeedBuff->IsRooted() || ( moveSpeedBuff->GetSecsToEnd() < 0.03 && moveSpeedBuff->IsAutoRemove() );
        }

        if ( needsNew )
        {
            std::auto_ptr<Buff> buff( core::BuffFactory::Get()( MoveSpeedBuff::GetType_static() ) );
            MoveSpeedBuff* moveSpeedBuff = ( MoveSpeedBuff* )buff.get();
            moveSpeedBuff->SetRooted( true );
            moveSpeedBuff->SetFlatBonus( 0 );
            moveSpeedBuff->SetPercentBonus( 0.0 );
            moveSpeedBuff->SetAutoRemove( true );
            moveSpeedBuff->SetSecsToEnd( weapon->ChargeTime() / 8.9 );
            buffHolderC->AddBuff( buff );
        }
    }
    if ( mProgramState.mMode == core::ProgramState::Client )
    {
        return;
    }

    if ( weapon->IsCharging() )
    {
        Opt<IAudibleComponent> ac = actor.Get<IAudibleComponent>();
        if( ac.IsValid() )
        {
            static int32_t const loop = AutoId( "gauss_alt_charge" );
            ac->AddLoopingEffect( loop );
        }
    }

    if ( weapon->GetCooldown() > 0 )
    {
        return;
    }
    Opt<IAudibleComponent> ac = actor.Get<IAudibleComponent>();
    if ( weapon->IsShooting() )
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory( weapon->GetShotId() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );

        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), false );
        if( ac.IsValid() )
        {
            ac->AddOneShotEffect( weapon->GetShotId() );
        }
    }
    else if ( weapon->IsShootingAlt() )
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory( weapon->GetShotAltId() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );

        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), true );
        if( ac.IsValid() )
        {
            ac->AddOneShotEffect( weapon->GetShotAltId() );
        }
    }
}

} // namespace engine

