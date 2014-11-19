#ifndef INCLUDED_CORE_ITEM_H
#define INCLUDED_CORE_ITEM_H

#include "core/item_repo.h"
#include "platform/log.h"

class Actor;
class Item
{
public:
    enum ItemType
    {
        Normal,
        Weapon
    };
    virtual ~Item()
    {
        LOG( "Item dead:%i", mId );
    }
    virtual void SetActor( Actor* Obj );
    virtual void Update( double Seconds );
    ItemType GetType() const
    {
        return mType;
    }
protected:
    int32_t mId;
    Actor* mActor;
    ItemType mType;

    friend class platform::Factory<Item>;
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
    virtual void Update( double Seconds ) ;
    friend class platform::Factory<Item>;
};
#endif//INCLUDED_CORE_ITEM_H
