#ifndef INCLUDED_CORE_BLUE_GRENADE_NORMAL_ITEM_H
#define INCLUDED_CORE_BLUE_GRENADE_NORMAL_ITEM_H

#include "normal_item.h"
#include "core/property_loader.h"
#include "platform/export.h"

class BlueGrenadeNormalItem : public NormalItem
{
public:
    BlueGrenadeNormalItem( int32_t id );
protected:
    BlueGrenadeNormalItem();
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void BlueGrenadeNormalItem::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<NormalItem>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( BlueGrenadeNormalItem, BlueGrenadeNormalItem, "blue_grenade_normal_item" );
#endif//INCLUDED_CORE_BLUE_GRENADE_NORMAL_ITEM_H

//command:  "classgenerator.exe" -g "normal_item" -c "blue_grenade_normal_item"
