#include "engine/items/gatling_gun_weapon_sub_system.h"
#include "core/gatling_gun.h"

namespace engine {

GatlingGunWeaponSubSystem::GatlingGunWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mShotId( AutoId( "gatling_gun_projectile" ) )
    , mProgramState( core::ProgramState::Get())
{
}

void GatlingGunWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void GatlingGunWeaponSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<GatlingGun> weapon = inventoryC->GetSelectedWeapon();
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();

    GatlingGun::DeployState deployState=weapon->GetDeployState();
    if (weapon->GetReloadTime()<=0.0&&weapon->GetShoot()&&(deployState==GatlingGun::Deployed||deployState==GatlingGun::Undeployed))
    {
        // shooting has a windup time before actual shots come out. deployed or undelpoyed state is needed
        weapon->SetWindup(
            std::min(weapon->GetWindup()+weapon->GetWindupSpeed()*DeltaTime,weapon->GetWindupMax()));
    } 
    else
    {
        // not shooting raises back the windup time.
        weapon->SetWindup(
            std::max(weapon->GetWindup()-weapon->GetWindupSpeed()*DeltaTime,0.0));
    }
    if (weapon->GetShootAlt())
    {
        if (deployState==GatlingGun::Deployed)
        {
            deployState=GatlingGun::Undeploying;
        }
        else if (deployState==GatlingGun::Undeployed)
        {
            deployState=GatlingGun::Deploying;
            if (moveC.IsValid())
            {
                moveC->SetRooted(true);
            }
        }
    }
    if (deployState==GatlingGun::Deploying)
    {
        weapon->SetDeploy(
            std::min(weapon->GetDeploy()+weapon->GetDeploySpeed()*DeltaTime,weapon->GetDeployMax()));
        if (weapon->GetDeploy()==weapon->GetDeployMax())
        {
            deployState=GatlingGun::Deployed;
        }
    }
    else if (deployState==GatlingGun::Undeploying)
    {
        weapon->SetDeploy(
            std::max(weapon->GetDeploy()-weapon->GetDeploySpeed()*DeltaTime,0.0));
        if (weapon->GetDeploy()==0.0)
        {
            deployState=GatlingGun::Undeployed;
            if (moveC.IsValid())
            {
                moveC->SetRooted(false);
            }

        }
    }
    weapon->SetDeployState(deployState);
    if (mProgramState.mMode==core::ProgramState::Client||weapon->GetCooldown()>0)
    {
        return;
    }
    if (weapon->IsShooting())
    {
        //EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),false);
    }
    else if (weapon->IsShootingAlt())
    {
        //EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),true);
    }
}

} // namespace engine

