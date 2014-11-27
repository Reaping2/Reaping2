#ifndef INCLUDED_CORE_I_INVENTORY_COMPONENT_H
#define INCLUDED_CORE_I_INVENTORY_COMPONENT_H
#include "component.h"
#include <boost/ptr_container/ptr_list.hpp>
#include "core/item.h"

class IInventoryComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IInventoryComponent)
    typedef boost::ptr_list<Item> ItemList_t;
    virtual ItemList_t const& GetItems()const=0;
    virtual void AddItem( int32_t Id )=0;
    virtual void DropItemType( Item::ItemType Type )=0;
    virtual void Update( double Seconds )=0;
protected:
    friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_INVENTORY_COMPONENT_H