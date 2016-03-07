#ifndef INCLUDED__ITEM_TYPE_H
#define INCLUDED__ITEM_TYPE_H

#include "platform/singleton.h"
#include "boost/bimap.hpp"

class ItemType : public platform::Singleton<ItemType>
{
protected:
    friend class platform::Singleton<ItemType>;
    ItemType();
public:
    enum Type
    {
        Normal = 0,
        Weapon,
        Buff,
        Num_Classes
    };
    ItemType::Type operator()( int32_t Id ) const;
    int32_t operator()( ItemType::Type type ) const;
private:
    typedef boost::bimap<int32_t, ItemType::Type> IdToItemTypeMap_t;
    IdToItemTypeMap_t mIdToItemTypeMap;
};

#endif//INCLUDED__ITEM_TYPE_H


//command:  "classgenerator.exe" -g "enum" -c "item_type" -m "Normal-normal Weapon-weapon Buff-buff"
