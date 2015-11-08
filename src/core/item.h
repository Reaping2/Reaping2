#ifndef INCLUDED_CORE_ITEM_H
#define INCLUDED_CORE_ITEM_H

#include "core/item_factory.h"
#include "platform/log.h"
#include "core/actor.h"
#include "item_type.h"

class Item
{
public:
    virtual ~Item()
    {
        LOG( "Item dead:%i", mId );
    }
    virtual void SetActor( Actor* Obj );
    ItemType::Type GetType() const
    {
        return mType;
    }
protected:
    int32_t mId;
    Actor* mActor;
    ItemType::Type mType;

    friend class ItemFactory;
    Item( int32_t Id );

public:
    double mState;
    double GetState()const
    {
        return mState;
    }
    void SetState( double S )
    {
        mState = S;
    }
    int32_t GetId() const
    {
        return mId;
    }
};

class DefaultItem : public Item
{
public:
    DefaultItem( int32_t Id );
    ~DefaultItem() {};
    friend class ItemFactory;
};
#endif//INCLUDED_CORE_ITEM_H
