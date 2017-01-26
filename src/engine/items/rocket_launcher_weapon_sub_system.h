#ifndef INCLUDED_ENGINE_ITEMS_ROCKET_LAUNCHER_WEAPON_SUB_SYSTEM_H
#define INCLUDED_ENGINE_ITEMS_ROCKET_LAUNCHER_WEAPON_SUB_SYSTEM_H

#include "engine/items/common_sub_system_includes.h"

namespace engine {

class RocketLauncherWeaponSubSystem : public SubSystem, public SubSystemHolder
{
public:
    DEFINE_SUB_SYSTEM_BASE( RocketLauncherWeaponSubSystem )

    RocketLauncherWeaponSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );

private:
    Scene& mScene;
    Opt<WeaponItemSubSystem> mWeaponItemSubSystem;
    ActorFactory& mActorFactory;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ITEMS_ROCKET_LAUNCHER_WEAPON_SUB_SYSTEM_H
