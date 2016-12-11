#include "platform/i_platform.h"
#include "engine/engine.h"
#include "engine/controllers/scripted_controller_sub_system.h"
#include "core/scripted_controller/scripted_controller_component.h"

namespace engine {

ScriptedControllerSubSystem::ScriptedControllerSubSystem()
    : mScene(Scene::Get())
    , mProgramState(core::ProgramState::Get())
{
}


void ScriptedControllerSubSystem::Init()
{
}


void ScriptedControllerSubSystem::Update(Actor& actor, double DeltaTime)
{
    using namespace scriptedcontroller;
    Opt<ScriptedControllerComponent> scriptedCC=actor.Get<IControllerComponent>();
    if (!scriptedCC.IsValid()||!scriptedCC->IsEnabled())
    {
        return;
    }
}


} // namespace engine

