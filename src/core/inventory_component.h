#ifndef INCLUDED_CORE_INVENTORY_COMPONENT_H
#define INCLUDED_CORE_INVENTORY_COMPONENT_H
#include "core/i_inventory_component.h"
#include "core/item_factory.h"
#include "core/property_loader.h"
#include "core/opt.h"
#include "core/weapon.h"

class InventoryComponent : public IInventoryComponent
{
public:
    virtual ItemList_t const& GetItems()const;
    virtual ItemList_t& GetItems();
    virtual void AddItem( int32_t Id );
    virtual Opt<Item> GetItem( int32_t Id );
    virtual void DropItemType( Item::ItemType Type );
    virtual void Update( double Seconds );
    virtual Opt<Weapon> GetSelectedWeapon();
    virtual void SetSelectedWeapon( int32_t Id );
    virtual void SetActor(Actor* Obj);
    virtual Opt<NormalItem> GetSelectedNormalItem();
    virtual void SetSelectedNormalItem( int32_t Id );
    virtual ~InventoryComponent();
protected:
    InventoryComponent();
    friend class ComponentFactory;
private:
    ItemFactory& mItemFactory;
    ItemList_t mItems;
    Opt<Weapon> mSelectedWeapon;
    Opt<NormalItem> mSelectedNormalItem;
};

class InventoryComponentLoader: public ComponentLoader<InventoryComponent>
{
    virtual void BindValues();
protected:
    InventoryComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_INVENTORY_COMPONENT_H