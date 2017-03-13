#ifndef INCLUDED_CORE_ITEM_H
#define INCLUDED_CORE_ITEM_H

#include "core/item_factory.h"
#include "platform/log.h"
#include "core/actor.h"
#include "item_type.h"
#include <portable_oarchive.hpp>
#include <portable_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include "item_loader.h"
#include "platform/export.h"

class Item
{
public:
    ItemType::Type GetType() const;
    int32_t GetActorGUID() const;
    double GetState()const;
    void SetState( double S );
    int32_t GetId() const;

    virtual void SetActorGUID( int32_t actorGUID );
    virtual bool CanSwitch() const;
    virtual void Deselected();
    virtual void Selected();
    virtual ~Item();
protected:
    int32_t mId;
    int32_t mActorGUID;
    ItemType::Type mType;
    double mState;

    friend class ItemFactory;
    Item( int32_t Id );
    Item();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void Item::serialize( Archive& ar, const unsigned int version )
{
    ar& mId;
    ar& mType;
    ar& mState;
    ar& mActorGUID;
}

class DefaultItem : public Item
{
public:
    DefaultItem( int32_t Id );
    ~DefaultItem() {};
};

class DefaultItemLoader: public ItemLoader<DefaultItem>
{
public:
    virtual void BindValues();
    DefaultItemLoader();
};

#endif//INCLUDED_CORE_ITEM_H
