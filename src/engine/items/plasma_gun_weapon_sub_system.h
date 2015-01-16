#ifndef INCLUDED_ENGINE_ITEMS_PLASMA_GUN_WEAPON_SUB_SYSTEM_H
#define INCLUDED_ENGINE_ITEMS_PLASMA_GUN_WEAPON_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"
#include "core/opt.h"
#include "engine/sub_system_holder.h"
#include "weapon_item_sub_system.h"
#include "core/actor_factory.h"

namespace engine {

class PlasmaGunWeaponSubSystem : public SubSystem, public SubSystemHolder
{
public:
    DEFINE_SUB_SYSTEM_BASE(PlasmaGunWeaponSubSystem)

    PlasmaGunWeaponSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
    Opt<WeaponItemSubSystem> mWeaponItemSubSystem;
    ActorFactory& mActorFactory;
    int32_t mShotId;
    int32_t mAltShotId;
    int32_t mPlasmaShotId;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ITEMS_PLASMA_GUN_WEAPON_SUB_SYSTEM_H
