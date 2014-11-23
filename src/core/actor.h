#ifndef INCLUDED_CORE_ACTOR_H
#define INCLUDED_CORE_ACTOR_H

#include "core/i_collision_component.h"
#include "core/action_holder.h"
using namespace ::boost::multi_index;
class Action;
class Item;

class Actor : public AutoId, public ComponentHolder, public ActionHolder
{
public:
protected:
    int32_t mGUID;
public:
    //TODO: only one addaction
    void AddAction( int32_t Id );
    virtual void AddAction( std::auto_ptr<Action> Act );
    Actor( std::string const& Name );
    virtual ~Actor();

    int32_t GetGUID()const
    {
        return mGUID;
    }

    virtual void Update( double Seconds );
};



#endif//INCLUDED_CORE_ACTOR_H
