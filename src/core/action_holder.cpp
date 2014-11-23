#include "action_holder.h"
#include "i_core.h"

ActionHolder::ActionHolder()
    : mActionFactory( ActionRepo::Get() )
{

}

bool ActionHolder::CanAddAction( int32_t Id )const
{
    for( ActionList_t::const_iterator i = mActions.begin(), e = mActions.end(); i != e; ++i )
        if( i->second->Blocks( Id ) )
        {
            return false;
        }
    return true;
}



//TODO: there will be only one
void ActionHolder::AddAction( std::auto_ptr<Action> Act )
{
    if( !CanAddAction( Act->GetId() ) )
    {
        return;
    }
    ActionList_t::iterator i = mActions.find( Act->GetId() );
    if( i == mActions.end() )
    {
        if ( Act->Activate() )
        {
            mActions.insert( Act->GetId(), Act );
        }
    }
    else
    {
        //TODO: this will be changed
        Action* a = i->second;
        a->Activate();
    }
}


void ActionHolder::DropAction( int32_t Id )
{
    ActionList_t::iterator i = mActions.find( Id );
    if( i == mActions.end() )
    {
        return;
    }
    i->second->Deactivate();
    mActions.erase( i );
}

ActionHolder::~ActionHolder()
{
}

bool ActionHolder::HasAction( int32_t Id ) const
{
    return mActions.find( Id ) != mActions.end();
}

ActionHolder::ActionList_t const& ActionHolder::GetActions() const
{
    return mActions;
}
