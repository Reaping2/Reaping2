#ifndef INCLUDED_CORE_INVENTORY_COMPONENT_H
#define INCLUDED_CORE_INVENTORY_COMPONENT_H
#include "core/i_inventory_component.h"
#include "core/item_factory.h"

class InventoryComponent : public IInventoryComponent
{
public:
    virtual ItemList_t const& GetItems()const;
    virtual void AddItem( int32_t Id );
    virtual void DropItemType( Item::ItemType Type );
    virtual void SetActor( Actor* Obj );
    virtual void Update( double Seconds );
protected:
    InventoryComponent();
    friend class ComponentFactory;
private:
    Actor* mActor;
    ItemFactory& mItemFactory;
    ItemList_t mItems;
};

#endif//INCLUDED_CORE_INVENTORY_COMPONENT_H