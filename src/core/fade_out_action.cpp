#include "i_core.h"
#include "core/i_health_component.h"

FadeOutAction::FadeOutAction( int32_t Id )
    : Action( Id )
{
    mIsSelfDestruct = true;
    mIsLoop = false;
    mSecsToEnd = 20;
    mIsRefresh = false;
}

void FadeOutAction::Deactivate()
{
    if( !mActor )
    {
        return;
    }
    Opt<IHealthComponent> healthC = mActor->Get<IHealthComponent>();
    if ( healthC.IsValid() )
    {
        healthC->SetHP( 0 );
    }
}

