#ifndef INCLUDED_ENGINE_FLASH_NORMAL_ITEM_SUB_SYSTEM_H
#define INCLUDED_ENGINE_FLASH_NORMAL_ITEM_SUB_SYSTEM_H

#include "engine/items/common_sub_system_includes.h"
#include "normal_item_sub_system.h"

namespace engine {

class FlashNormalItemSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( FlashNormalItemSubSystem )
    FlashNormalItemSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
    Opt<NormalItemSubSystem> mNormalItemSubSystem;
    ActorFactory& mActorFactory;
};

} // namespace engine

#endif//INCLUDED_ENGINE_FLASH_NORMAL_ITEM_SUB_SYSTEM_H

