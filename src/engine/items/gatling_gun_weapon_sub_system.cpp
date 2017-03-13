#include "engine/items/gatling_gun_weapon_sub_system.h"
#include "core/gatling_gun.h"
#include "core/buffs/move_speed_buff.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"
#include "core/i_audible_component.h"

namespace engine {

GatlingGunWeaponSubSystem::GatlingGunWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mWeaponItemSubSystem( WeaponItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
{
}

void GatlingGunWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void GatlingGunWeaponSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<GatlingGun> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    Opt<IAudibleComponent> ac = actor.Get<IAudibleComponent>();

    GatlingGun::DeployState deployState = weapon->GetDeployState();
    auto windup = weapon->GetWindup();
    if ( weapon->GetReloadTime() <= 0.0 && weapon->GetShoot() && ( deployState == GatlingGun::Deployed || deployState == GatlingGun::Undeployed ) )
    {
        // shooting has a windup time before actual shots come out. deployed or undelpoyed state is needed
        weapon->SetWindup(
            std::min( windup + DeltaTime, weapon->GetWindupMax() ) );
        if( windup != weapon->GetWindup() && ac.IsValid() && windup >= weapon->GetWindupMax() * 0.35 )
        {
            static int32_t const loop = AutoId( "gatling_up" );
            ac->AddLoopingEffect( loop );
        }
    }
    else
    {
        // not shooting raises back the windup time.
        weapon->SetWindup(
            std::max( windup - DeltaTime, 0.0 ) );
        if( windup != weapon->GetWindup() && ac.IsValid() )
        {
            static int32_t const loop = AutoId( "gatling_down" );
            ac->AddLoopingEffect( loop );
        }
    }
    if ( weapon->GetShootAlt() && weapon->GetReloadTime() <= 0 )
    {
        if ( deployState == GatlingGun::Deployed )
        {
            deployState = GatlingGun::Undeploying;
        }
        else if ( deployState == GatlingGun::Undeployed )
        {
            deployState = GatlingGun::Deploying;
        }
    }
    if ( deployState == GatlingGun::Deploying )
    {
        weapon->SetDeploy(
            std::min( weapon->GetDeploy() + DeltaTime, weapon->GetDeployMax() ) );
        if ( weapon->GetDeploy() == weapon->GetDeployMax() )
        {
            deployState = GatlingGun::Deployed;
        }
    }
    else if ( deployState == GatlingGun::Undeploying )
    {
        weapon->SetDeploy(
            std::max( weapon->GetDeploy() - DeltaTime, 0.0 ) );
        if ( weapon->GetDeploy() == 0.0 )
        {
            deployState = GatlingGun::Undeployed;
        }
    }
    weapon->SetDeployState( deployState );
    if ( moveC.IsValid() && deployState != GatlingGun::Undeployed )
    {
        Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
        if( buffHolderC.IsValid() )
        {
            bool needsNew = true;
            BuffListFilter<IBuffHolderComponent::All> buffListFilter( buffHolderC->GetBuffList(), MoveSpeedBuff::GetType_static() );
            for( BuffListFilter<IBuffHolderComponent::All>::const_iterator moveSpeedBuffIt = buffListFilter.begin(), moveSpeedBuffE = buffListFilter.end(); needsNew && moveSpeedBuffIt != moveSpeedBuffE; ++moveSpeedBuffIt )
            {
                Opt<MoveSpeedBuff> moveSpeedBuff( *moveSpeedBuffIt );
                needsNew = !moveSpeedBuff->IsRooted() || ( moveSpeedBuff->GetSecsToEnd() < 0.1 && moveSpeedBuff->IsAutoRemove() );
            }
            if( needsNew )
            {
                std::auto_ptr<Buff> buff( core::BuffFactory::Get()( MoveSpeedBuff::GetType_static() ) );
                MoveSpeedBuff* moveSpeedBuff = ( MoveSpeedBuff* )buff.get();
                moveSpeedBuff->SetRooted( true );
                moveSpeedBuff->SetFlatBonus( 0 );
                moveSpeedBuff->SetPercentBonus( 0.0 );
                moveSpeedBuff->SetAutoRemove( true );
                moveSpeedBuff->SetSecsToEnd( weapon->GetDeployMax() / 3.9 );
                buffHolderC->AddBuff( buff );
            }
        }
    }
    if ( mProgramState.mMode == core::ProgramState::Client || weapon->GetCooldown() > 0 )
    {
        return;
    }
    if ( weapon->IsShooting() )
    {
        if( ac.IsValid() )
        {
            static int32_t const loop = AutoId( "gatling_shot_loop" );
            static int32_t const windLoop = AutoId( "gatling_wind_loop" );
            ac->AddLoopingEffect( loop );
            ac->AddLoopingEffect( windLoop );
        }

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory( weapon->GetShotId() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );

        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), false );
    }
    else if ( weapon->IsShootingAlt() )
    {
        if( ac.IsValid() )
        {
            static int32_t const loop = AutoId( "gatling_shot_loop" );
            static int32_t const windLoop = AutoId( "gatling_wind_loop" );
            ac->AddLoopingEffect( loop );
            ac->AddLoopingEffect( windLoop );
        }


        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory( weapon->GetShotAltId() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );

        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), true );
    }
}

} // namespace engine

