#include "engine/items/rocket_launcher_weapon_sub_system.h"

namespace engine
{

	RocketLauncherWeaponSubSystem::RocketLauncherWeaponSubSystem()
		: SubSystemHolder()
		, mScene(Scene::Get())
		, mWeaponItemSubSystem(WeaponItemSubSystem::Get())
		, mActorFactory(ActorFactory::Get())
		, mShotId(AutoId("rocket_launcher_primary"))
		, mAltShotId(AutoId("rocket_launcher_secondary"))
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
		if(!weapon->IsShoot() && !weapon->IsShootAlt())
		{
			// not firing, nothing to do
			return;
		}
		if(weapon->GetCooldown()>0)
			return;

		int32_t IdToSend;
		int32_t ScatterToSend;
		WeaponItemSubSystem::Projectiles_t projectiles;
		std::auto_ptr<Actor> rocket = mActorFactory(mRocketId);

		if(weapon->IsShoot())
		{
			IdToSend = mShotId;
			ScatterToSend = weapon->GetScatter();
		}
		else if(weapon->IsShootAlt())
		{
			IdToSend = mAltShotId;
			ScatterToSend = weapon->GetAltScatter();
			// TODO: how to make it homing?
		}
		else
		{
			BOOST_ASSERT(false);
		}

		EventServer<AudibleEvent>::Get().SendEvent(AudibleEvent(IdToSend));
		projectiles.push_back(rocket.release());
		mWeaponItemSubSystem->AddProjectiles(actor,projectiles,ScatterToSend);
		
	}

} // namespace engine
