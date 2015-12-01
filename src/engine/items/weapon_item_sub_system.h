#ifndef INCLUDED_ENGINE_ITEMS_WEAPON_ITEM_SUB_SYSTEM_H
#define INCLUDED_ENGINE_ITEMS_WEAPON_ITEM_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"
#include "core/opt.h"
#include "engine/sub_system_holder.h"
#include "core/weapon.h"
#include "core/program_state.h"
#include "core/shot_event.h"

namespace engine {

class WeaponItemSubSystem : public SubSystem, public SubSystemHolder
{
public:
    DEFINE_SUB_SYSTEM_BASE(WeaponItemSubSystem)

    WeaponItemSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    typedef std::list<Opt<Actor> > Projectiles_t;

    void AddProjectiles( Actor& actor, Projectiles_t& projectiles, Scatter& scatter, bool alt=false);
    static Opt<WeaponItemSubSystem> Get();
    static void SetProjectilePosition(Actor& projectile, Actor& actor);
private:
    Scene& mScene;
    core::ProgramState& mProgramState;
    AutoReg mOnShot;
    void OnShot(core::ShotEvent const& Evt);
};

} // namespace engine

#endif//INCLUDED_ENGINE_ITEMS_WEAPON_ITEM_SUB_SYSTEM_H
