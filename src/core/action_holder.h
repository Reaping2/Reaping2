#ifndef INCLUDED_CORE_ACTION_HOLDER_H
#define INCLUDED_CORE_ACTION_HOLDER_H

#include "core/action.h"
#include "core/action_repo.h"

#include <memory>
#include <boost/ptr_container/ptr_map.hpp>


//class Action;
class ActionHolder
{
public:
    typedef boost::ptr_map<int32_t, Action> ActionList_t;

    ActionHolder();
    ActionList_t const& GetActions()const;

    bool CanAddAction( int32_t Id )const;
    virtual void AddAction( std::auto_ptr<Action> Act );
    bool HasAction( int32_t Id )const;
    void DropAction( int32_t Id );

    virtual ~ActionHolder();
protected:
    ActionRepo& mActionFactory;
    ActionList_t mActions;
};
#endif//INCLUDED_CORE_ACTION_HOLDER_H
