#include "engine/items/rocket_launcher_weapon_sub_system.h"

namespace engine
{

    RocketLauncherWeaponSubSystem::RocketLauncherWeaponSubSystem()
        : SubSystemHolder()
          , mScene(Scene::Get())
          , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
          , mActorFactory(ActorFactory::Get())
          , mShotId(AutoId("rocket_launcher_primary"))
          , mAltShotId(AutoId("rocket_launcher_target_projectile"))
          , mRocketId(AutoId("rocket_launcher_projectile"))
    {
    }

    void RocketLauncherWeaponSubSystem::Init()
    {
        SubSystemHolder::Init();
    }

    void RocketLauncherWeaponSubSystem::Update( Actor& actor, double DeltaTime )
    {
        Opt<Weapon> weapon = actor.Get<IInventoryComponent>()->GetSelectedWeapon();
        if(!weapon->IsShooting() && !weapon->IsShootingAlt())
        {
            // not firing, nothing to do
            return;
        }
        if(weapon->GetCooldown()>0)
            return;

        int32_t IdToSend;
        WeaponItemSubSystem::Projectiles_t projectiles;
        std::auto_ptr<Actor> rocket = mActorFactory(mRocketId);

        if(weapon->IsShooting())
        {
            IdToSend = mShotId;
        }
        else if(weapon->IsShootingAlt())
        {
            IdToSend = mAltShotId;
            rocket=mActorFactory(mAltShotId);
        }
        else
        {
            BOOST_ASSERT(false);
        }

        projectiles.push_back(rocket.release());
        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),weapon->IsShootingAlt());
    }

} // namespace engine
