#ifndef INCLUDED_ENGINE_ITEMS_INVENTORY_SYSTEM_H
#define INCLUDED_ENGINE_ITEMS_INVENTORY_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "engine/sub_system_holder.h"

namespace engine {

class InventorySystem : public System, public SubSystemHolder
{
public:
    DEFINE_SYSTEM_BASE(InventorySystem)
    InventorySystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ITEMS_INVENTORY_SYSTEM_H

