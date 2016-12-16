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
    if (!scriptedCC.IsValid()||!scriptedCC->IsEnabled()||scriptedCC->GetStates().empty())
    {
        return;
    }
    auto state = scriptedCC->GetState();
    if (!state.IsValid())
    {
        scriptedCC->SetStateIdentifier( scriptedCC->GetStates().begin()->first );
        for (auto st : scriptedCC->GetStates())
        {
            if (st.second.IsStart())
            {
                scriptedCC->SetStateIdentifier( st.first );
                break;
            }
        }
        state = scriptedCC->GetState();
    }
    state->UpdateTransitions( actor, DeltaTime );
    int32_t nextStateId = state->GetNextStateIdentifier();
    if (nextStateId != scriptedCC->GetStateIdentifier())
    {
        state->Reset( actor );
        scriptedCC->SetStateIdentifier( nextStateId );
        state = scriptedCC->GetState();
    }
    state->Update( actor, DeltaTime );
}


} // namespace engine

