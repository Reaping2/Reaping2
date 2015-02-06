#ifndef INCLUDED_CORE_ACTOR_H
#define INCLUDED_CORE_ACTOR_H

#include "platform/i_platform.h"
#include "core/component.h"

using namespace ::boost::multi_index;

class ComponentHolder;
class Actor : public ComponentHolder
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
    virtual void AddComponent( std::auto_ptr<Component> Comp  );
    virtual ~Actor();

    // dont use it pls its for network guid sync
    void SetGUID(int32_t guid)
    {
        mGUID=guid;
    }

    int32_t GetGUID()const
    {
        return mGUID;
    }

};



#endif//INCLUDED_CORE_ACTOR_H
