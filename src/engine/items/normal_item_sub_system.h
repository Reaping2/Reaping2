#ifndef INCLUDED_ENGINE_ITEMS_NORMAL_ITEM_SUB_SYSTEM_H
#define INCLUDED_ENGINE_ITEMS_NORMAL_ITEM_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"
#include "core/opt.h"
#include "engine/sub_system_holder.h"

namespace engine {

class NormalItemSubSystem : public SubSystem, public SubSystemHolder
{
public:
    DEFINE_SUB_SYSTEM_BASE( NormalItemSubSystem )
    NormalItemSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    static Opt<NormalItemSubSystem> Get();
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ITEMS_NORMAL_ITEM_SUB_SYSTEM_H
