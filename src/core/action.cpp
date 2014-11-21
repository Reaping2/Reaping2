#include "i_core.h"

DefaultAction::DefaultAction( int32_t Id )
    : Action( Id )
{
    mIsRefresh = false;
    mIsLoop = false;
    mIsSelfDestruct = true;
    mAreBlockedActionsExcluded = false;
    mAreCancelledActionsExcluded = false;
    mSecsToEnd = 0;
}

void Action::SetActor( Actor* Obj )
{
    mActor = Obj;
}
bool DefaultAction::Activate()
{
    return true;
}

void DefaultAction::Deactivate()
{
}

void DefaultAction::Update( double Seconds )
{
}


Action::Action( int32_t Id )
    : mId( Id )
    , mIsRefresh( false )
    , mIsLoop( false )
    , mIsSelfDestruct( false )
    , mAreBlockedActionsExcluded( false )
    , mAreCancelledActionsExcluded( false )
    , mSecsToEnd( 1 )
    , mState( 0 )
{
}

Action::~Action()
{
}

int32_t Action::GetId() const
{
    return mId;
}

bool Action::Activate()
{
    if( !mActor )
    {
		//TODO: Action currently cancelled if ComponentDependency fails further component development needed
        return false;
    }
    if( mIsRefresh )
    {
        mState = 0;
    }
    Actor::ActionList_t const& Actions = mActor->GetActions();
    //if this action cancels others
    for( Actor::ActionList_t::const_iterator i = Actions.begin(), e = Actions.end(); i != e; )
    {
        // increment iterator before dropping
        int32_t Id = ( i++ )->first;
        if( Cancels( Id ) )
        {
            mActor->DropAction( Id );
        }
    }
    return true;
}

void Action::Deactivate()
{
}

bool Action::Blocks( int32_t What ) const
{
    return mAreBlockedActionsExcluded ^ ( std::find( mBlockedActionIds.begin(), mBlockedActionIds.end(), What ) != mBlockedActionIds.end() );
}

bool Action::Cancels( int32_t What ) const
{
    return mAreCancelledActionsExcluded ^ ( std::find( mCancelledActionIds.begin(), mCancelledActionIds.end(), What ) != mCancelledActionIds.end() );
}

void Action::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
    double nextState = mSecsToEnd==0?100:(mState + 1. / mSecsToEnd * Seconds * 100.);
    
    if( nextState >= 100 )
    {
        if( mIsLoop )
        {
            nextState = fmod( nextState, 100. );
        }
        else if( mIsSelfDestruct )
        {
            mActor->DropAction( mId );
            return;
        }
        else
        {
            nextState = 100.;
        }
    }
    mState = nextState;
}
