#ifndef INCLUDED_CORE_CLOAK_NORMAL_ITEM_H
#define INCLUDED_CORE_CLOAK_NORMAL_ITEM_H

#include "normal_item.h"
#include "core/property_loader.h"

class CloakNormalItem : public NormalItem
{
public:
    CloakNormalItem( int32_t id );
protected:
    CloakNormalItem();
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void CloakNormalItem::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<NormalItem>(*this);
}

#endif//INCLUDED_CORE_CLOAK_NORMAL_ITEM_H

//command:  "classgenerator.exe" -g "normal_item" -c "cloak_normal_item"
