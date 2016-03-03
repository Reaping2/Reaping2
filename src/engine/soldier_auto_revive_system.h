#ifndef INCLUDED_ENGINE_SOLDIER_AUTO_REVIVE_SYSTEM_H
#define INCLUDED_ENGINE_SOLDIER_AUTO_REVIVE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/program_state.h"
#include "core/map/soldier_auto_revive_map_element_system.h"
#include "core/actor_event.h"
#include "secs_to_revive_event.h"

namespace engine {

class SoldierAutoReviveSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(SoldierAutoReviveSystem)
    SoldierAutoReviveSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    core::ProgramState& mProgramState;
    Opt<map::SoldierAutoReviveMapElementSystem> mSoldierAutoReviveMES;
    std::map<int32_t, double> mTimesOfDeath;
    ModelValue mAutoReviveTextModel;
    std::string const& GetText();
    std::string mAutoReviveText;
    double mSecsToRevive;
    AutoReg mOnActorEvent;
    void OnActorEvent(ActorEvent const& Evt);
    AutoReg mOnSecsToRevive;
    void OnSecsToRevive(engine::SecsToReviveEvent const& Evt);
};

} // namespace engine

#endif//INCLUDED_ENGINE_SOLDIER_AUTO_REVIVE_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "soldier_auto_revive_system"
