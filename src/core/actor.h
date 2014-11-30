#ifndef INCLUDED_CORE_ACTOR_H
#define INCLUDED_CORE_ACTOR_H

#include "platform/i_platform.h"
#include "core/i_collision_component.h"
//#include "core/action.h"
#include "core/item.h"
#include "core/action_holder.h"
#include "core/component.h"

using namespace ::boost::multi_index;

class ActionHolder;
class ComponentHolder;
class Actor : public ComponentHolder, public ActionHolder
{
public:
protected:
    int32_t mGUID;
    int32_t mId;
public:
    Actor( int32_t Id );
    int32_t GetId()const;
    //TODO: pickup needs this for render, but it will be removed
    void SetId( int32_t Id );
    //TODO: only one addaction
    void AddAction( int32_t Id );
    virtual void AddAction( std::auto_ptr<Action> Act );
    virtual void AddComponent( std::auto_ptr<Component> Comp  );
    virtual ~Actor();

    int32_t GetGUID()const
    {
        return mGUID;
    }

    virtual void Update( double Seconds );
};



#endif//INCLUDED_CORE_ACTOR_H
