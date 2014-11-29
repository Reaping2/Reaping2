#include "platform/i_platform.h"
#include "core/death_action.h"
#include "core/i_move_component.h"
#include "core/i_controller_component.h"
#include "core/actor.h" //TODO : this sucks


DeathAction::DeathAction( int32_t Id )
    : Action( Id )
{
    mAreBlockedActionsExcluded = true;
    mAreCancelledActionsExcluded = true;
    mIsSelfDestruct = false;
    mSecsToEnd = 0.7;
}

void DeathAction::Update( double Seconds )
{
    Action::Update( Seconds );
    if( !mActor )
    {
        return;
    }
    mActor->Get<IMoveComponent>()->SetSpeed( 0 );
}

bool DeathAction::Activate()
{
    if(Action::Activate())
    {
        Opt<IControllerComponent> controllerC = mActor->Get<IControllerComponent>();
        if(controllerC.IsValid())
        {
            controllerC->SetEnabled(false);
        }
        return true;
    }
    return false;
}
