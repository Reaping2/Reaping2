#ifndef INCLUDED_ENGINE_ITEMS_GRENADE_NORMAL_ITEM_SUB_SYSTEM_H
#define INCLUDED_ENGINE_ITEMS_GRENADE_NORMAL_ITEM_SUB_SYSTEM_H

#include "engine/items/common_sub_system_includes.h"
#include "normal_item_sub_system.h"

namespace engine {

class GrenadeNormalItemSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( GrenadeNormalItemSubSystem )

    GrenadeNormalItemSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );

    static void SetGrenadeProperties( Actor& Proj, Actor& actor );

private:
    Scene& mScene;
    Opt<NormalItemSubSystem> mNormalItemSubSystem;
    ActorFactory& mActorFactory;
    int32_t mProjectileId;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ITEMS_GRENADE_NORMAL_ITEM_SUB_SYSTEM_H
