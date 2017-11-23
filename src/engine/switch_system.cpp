#include "platform/i_platform.h"
#include "switch_system.h"
#include "core/i_switch_component.h"

namespace engine {

SwitchSystem::SwitchSystem()
    : mScene( Scene::Get() )
{
}


void SwitchSystem::Init()
{

    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<ISwitchComponent>().IsValid(); } );
}


void SwitchSystem::Update(double DeltaTime)
{
    for ( auto actor : mScene.GetActorsFromMap( GetType_static() ) )
    {
        Opt<ISwitchComponent> switchC = actor->Get<ISwitchComponent>();
        if (!switchC.IsValid())
        {
            continue;
        }
        switchC->SetStateChanged( false );
        if (switchC->GetState() == SwitchState::TransitionToOn || switchC->GetState() == SwitchState::TransitionToOff)
        {
            switchC->SetSecsToEnd( std::max( 0.0, switchC->GetSecsToEnd() - DeltaTime ) );
            if (switchC->GetSecsToEnd() <= 0.0)
            {
                switchC->SetState( switchC->GetState() == SwitchState::TransitionToOn?SwitchState::On:SwitchState::Off );
            }
        }
    }
}


} // namespace engine

