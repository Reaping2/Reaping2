#include "i_core.h"
#include "core/idle_action.h"

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