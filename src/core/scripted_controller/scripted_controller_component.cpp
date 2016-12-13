#include "core/scripted_controller/scripted_controller_component.h"
namespace scriptedcontroller {

ScriptedControllerComponent::ScriptedControllerComponent()
    : mStates()
    , mStateIdentifier( -1 )
{
}

void ScriptedControllerComponent::SetStates( States_t states )
{
    mStates = states;
}

ScriptedControllerComponent::States_t& ScriptedControllerComponent::GetStates()
{
    return mStates;
}

void ScriptedControllerComponent::SetStateIdentifier( int32_t StateIdentifier )
{
    mStateIdentifier = StateIdentifier;
}

int32_t ScriptedControllerComponent::GetStateIdentifier() const
{
    return mStateIdentifier;
}

Opt<State> ScriptedControllerComponent::GetState()
{
    return Opt<State>(mStateIdentifier>=0&&mStateIdentifier<mStates.size()?&mStates[mStateIdentifier]:nullptr);
}


void ScriptedControllerComponentLoader::BindValues()
{
    ScriptedControllerComponent::States_t states;
    Json::Value const& json = (*mSetters)["states"];
    if (!json.isArray())
    {
        return;
    }
    for (auto& part : json)
    {
        std::string str;
        int32_t identifier = 0;
        if (Json::GetStr( part["identifier"], str ))
        {
            identifier = AutoId( str );
            State state( identifier );
            state.Load( part );
            states.emplace( identifier, state );
        }
    }
    Bind<ScriptedControllerComponent::States_t>( &ScriptedControllerComponent::SetStates, states );
}

ScriptedControllerComponentLoader::ScriptedControllerComponentLoader()
{
}
} // namespace scriptedcontroller

REAPING2_CLASS_EXPORT_IMPLEMENT( scriptedcontroller__ScriptedControllerComponent, scriptedcontroller::ScriptedControllerComponent );

