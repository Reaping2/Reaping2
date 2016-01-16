#ifndef INCLUDED_CORE_ITEM_H
#define INCLUDED_CORE_ITEM_H

#include "core/item_factory.h"
#include "platform/log.h"
#include "core/actor.h"
#include "item_type.h"
#include <portable_oarchive.hpp>
#include <portable_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>

#define VIRTUAL_SERIALIZE \
    virtual void serialize( eos::portable_oarchive& ar ) const \
{ \
    ar << *this; \
} \
    virtual void serialize( eos::portable_iarchive& ar ) \
{ \
    ar >> *this; \
} \


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
    Actor* GetActor() const;
    VIRTUAL_SERIALIZE;
protected:
    int32_t mId;
    Actor* mActor;
    ItemType::Type mType;
    double mState;

    friend class ItemFactory;
    Item( int32_t Id );

public:
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
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void Item::serialize(Archive& ar, const unsigned int version)
{
    ar & mId;
    ar & mType;
    ar & mState;
}

class DefaultItem : public Item
{
public:
    DefaultItem( int32_t Id );
    ~DefaultItem() {};
    friend class ItemFactory;
};

#endif//INCLUDED_CORE_ITEM_H
