#ifndef INCLUDED_ENGINE_CHEST_SYSTEM_H
#define INCLUDED_ENGINE_CHEST_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/chest_changed_event.h"
#include "core/program_state.h"

namespace engine {

class ChestSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(ChestSystem)
    ChestSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene = Scene::Get();
    core::ProgramState& mProgramState = core::ProgramState::Get();
    AutoReg mOnChestChanged;
    void OnChestChanged( ChestChangedEvent const& Evt );
};

} // namespace engine

#endif//INCLUDED_ENGINE_CHEST_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "chest_component" -m "int32_t-pickup_profile Price-price"
