#include "i_core.h"

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
    mActor->SetHP( Actor::HP_DEAD );
}

