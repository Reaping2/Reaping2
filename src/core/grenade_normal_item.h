#ifndef INCLUDED_CORE_GRENADE_NORMAL_ITEM_H
#define INCLUDED_CORE_GRENADE_NORMAL_ITEM_H
#include "normal_item.h"
#include "platform/export.h"

class GrenadeNormalItem : public NormalItem
{
public:
    GrenadeNormalItem( int32_t Id );
protected:
    GrenadeNormalItem();
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void GrenadeNormalItem::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<NormalItem>(*this);
}


REAPING2_CLASS_EXPORT_KEY2(GrenadeNormalItem, GrenadeNormalItem,"grenade_normal_item");
#endif//INCLUDED_CORE_GRENADE_NORMAL_ITEM_H
