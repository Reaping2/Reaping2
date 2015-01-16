#ifndef INCLUDED_CORE_I_INVENTORY_COMPONENT_H
#define INCLUDED_CORE_I_INVENTORY_COMPONENT_H
#include "component.h"
#include "core/item.h"
#include "core/opt.h"
#include <list>
#include "weapon.h"

class IInventoryComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IInventoryComponent)
    typedef std::list<Opt<Item> > ItemList_t;
    virtual ItemList_t const& GetItems()const=0;
    virtual ItemList_t& GetItems()=0;
    virtual void AddItem( int32_t Id )=0;
    virtual Opt<Item> GetItem( int32_t Id )=0;
    virtual void DropItemType( Item::ItemType Type )=0;
    virtual Opt<Weapon> GetSelectedWeapon()=0;
    virtual void SetSelectedWeapon( int32_t Id )=0;
    virtual void Update( double Seconds )=0;
protected:
    friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_INVENTORY_COMPONENT_H