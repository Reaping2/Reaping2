#ifndef INCLUDED_CORE_ITEM_H
#define INCLUDED_CORE_ITEM_H

#include "core/item_factory.h"
#include "platform/log.h"
#include "core/actor.h"
#include "item_type.h"
#include <portable_oarchive.hpp>
#include <portable_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include "platform/export.h"

class Item
{
public:
    virtual ~Item()
    {
        LOG( "Item dead:%i", mId );
    }
    virtual void SetActorGUID( int32_t actorGUID );
    ItemType::Type GetType() const
    {
        return mType;
    }
    int32_t GetActorGUID() const;
protected:
    int32_t mId;
    int32_t mActorGUID;
    ItemType::Type mType;
    double mState;

    friend class ItemFactory;
    Item( int32_t Id );
    Item();
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
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void Item::serialize(Archive& ar, const unsigned int version)
{
    ar & mId;
    ar & mType;
    ar & mState;
    ar & mActorGUID;
}

class DefaultItem : public Item
{
public:
    DefaultItem( int32_t Id );
    ~DefaultItem() {};
    friend class ItemFactory;
};

#endif//INCLUDED_CORE_ITEM_H
