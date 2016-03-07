#ifndef INCLUDED_CORE_NORMAL_ITEM_H
#define INCLUDED_CORE_NORMAL_ITEM_H
#include "core/item.h"
#include "platform/export.h"

class NormalItem : public Item
{
public:
    NormalItem( int32_t Id );
    NormalItem();
    virtual void SetUse( bool use );
    bool IsUse() const;

    void SetConsumed( bool consumed );
    bool IsConsumed() const;

    bool mUse;
    bool mConsumed;
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void NormalItem::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Item>( *this );
    ar& mUse;
    ar& mConsumed;
}


REAPING2_CLASS_EXPORT_KEY2( NormalItem, NormalItem, "normal_item" );
#endif//INCLUDED_CORE_NORMAL_ITEM_H
