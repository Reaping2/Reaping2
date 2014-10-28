#include "i_core.h"

IdleAction::IdleAction( int32_t Id )
    : Action( Id )
{
    mAreBlockedActionsExcluded = false;
    mCancelledActionIds.push_back( IdStorage::Get().GetId( "move" ) );
    mSecsToEnd = 1;
    mIsLoop = true;
}

void IdleAction::Update( double Seconds )
{
    Action::Update( Seconds );
}