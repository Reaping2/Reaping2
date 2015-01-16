#ifndef INCLUDED_ENGINE_ITEMS_WEAPON_ITEM_SUB_SYSTEM_H
#define INCLUDED_ENGINE_ITEMS_WEAPON_ITEM_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"
#include "core/opt.h"
#include "engine/sub_system_holder.h"

namespace engine {

class WeaponItemSubSystem : public SubSystem, public SubSystemHolder
{
public:
    DEFINE_SUB_SYSTEM_BASE(WeaponItemSubSystem)

    WeaponItemSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    typedef std::list<Opt<Actor> > Projectiles_t;

    void AddProjectiles( Actor& actor, Projectiles_t& projectiles, uint32_t scatter);
    static Opt<WeaponItemSubSystem> Get();
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ITEMS_WEAPON_ITEM_SUB_SYSTEM_H
