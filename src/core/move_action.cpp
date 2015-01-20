#include "platform/i_platform.h"
#include "core/move_action.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/actor.h" //TODO : this sucks


MoveAction::MoveAction( int32_t Id )
    : Action( Id )
{
    mAreBlockedActionsExcluded = false;
    mCancelledActionIds.push_back( IdStorage::Get().GetId( "idle" ) );
    mSecsToEnd = 1;
    mIsLoop = true;
}

void MoveAction::Update( double Seconds )
{
}