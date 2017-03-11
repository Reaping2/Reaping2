#ifndef INCLUDED_ENGINE_CONTROLLERS_PLAYER_CONTROLLER_SUB_SYSTEM_H
#define INCLUDED_ENGINE_CONTROLLERS_PLAYER_CONTROLLER_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"
#include "input/i_input.h"
#include "core/opt.h"
#include "core/player_controller_component.h"
#include "core/program_state.h"
#include "input/input_system.h"
#include "core/item_type.h"

namespace engine {

class PlayerControllerSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( PlayerControllerSubSystem )
    PlayerControllerSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );

private:
    void SetSpeedAndHeading( Actor& actor, Opt<PlayerControllerComponent> playerControllerC );
    void SetOrientation( Actor& actor, Opt<PlayerControllerComponent> playerControllerC );
    void Shoot( Actor& actor, Opt<PlayerControllerComponent> playerControllerC );
    void HandleInputs( Actor& actor, Opt<PlayerControllerComponent> playerControllerC );
    void HandleReload( Actor& actor, Opt<PlayerControllerComponent> playerControllerC );
    void HandleItemSwitch( Actor& actor, Opt<PlayerControllerComponent> playerControllerC, ItemType::Type itemType );

    Scene& mScene;
    core::ProgramState& mProgramState;
    Opt<InputSystem> mInputSystem;
};

} // namespace engine

#endif//INCLUDED_ENGINE_CONTROLLERS_PLAYER_CONTROLLER_SUB_SYSTEM_H
