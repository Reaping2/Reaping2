#ifndef INCLUDED_ENGINE_BLUE_GRENADE_NORMAL_ITEM_SUB_SYSTEM_H
#define INCLUDED_ENGINE_BLUE_GRENADE_NORMAL_ITEM_SUB_SYSTEM_H

#include "engine/items/common_sub_system_includes.h"
#include "normal_item_sub_system.h"

namespace engine {

class BlueGrenadeNormalItemSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( BlueGrenadeNormalItemSubSystem )
    BlueGrenadeNormalItemSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
    Opt<NormalItemSubSystem> mNormalItemSubSystem;
    ActorFactory& mActorFactory;
    int32_t mProjectileId;
};

} // namespace engine

#endif//INCLUDED_ENGINE_BLUE_GRENADE_NORMAL_ITEM_SUB_SYSTEM_H

//command:  "classgenerator.exe" -g "normal_item_sub_system" -c "blue_grenade_normal_item_sub_system" --target_item_type "normal_item" --target_item_name "blue_grenade"
