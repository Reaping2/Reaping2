#include "i_core.h"

ShootAction::ShootAction( int32_t Id )
    : Action( Id )
{
    mAreBlockedActionsExcluded = false;
    mCancelledActionIds.push_back( AutoId( "shoot_alt" ) );
    mSecsToEnd = 1;
    mIsLoop = true;
    mIsSelfDestruct = false;
    mIsRefresh = false;
}

ShootAltAction::ShootAltAction( int32_t Id )
    : Action( Id )
{
    mAreBlockedActionsExcluded = false;
    mCancelledActionIds.push_back( AutoId( "shoot" ) );
    mSecsToEnd = 1;
    mIsLoop = true;
    mIsSelfDestruct = false;
    mIsRefresh = false;
}
