#ifndef INCLUDED_CORE_INVENTORY_COMPONENT_H
#define INCLUDED_CORE_INVENTORY_COMPONENT_H
#include "core/i_inventory_component.h"
#include "core/item_factory.h"
#include "core/property_loader.h"
#include "core/opt.h"
#include "core/weapon.h"
#include <boost/serialization/list.hpp>
#include <boost/serialization/export.hpp>
class InventoryComponent : public IInventoryComponent
{
public:
    virtual ItemList_t const& GetItems()const;
    virtual ItemList_t& GetItems();
    virtual void AddItem( int32_t Id );
    virtual void DropItem( int32_t Id );
    virtual Opt<Item> GetItem( int32_t Id );
    virtual void DropItemType( ItemType::Type Type );
    virtual void Update( double Seconds );
    virtual Opt<Weapon> GetSelectedWeapon();
    virtual void SetSelectedWeapon( int32_t Id );
    virtual void SetActorGUID(int32_t actorGUID);
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
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void InventoryComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IInventoryComponent>(*this);
    ar & mItems;
    ar & mSelectedWeapon;
    ar & mSelectedNormalItem;
}

class InventoryComponentLoader: public ComponentLoader<InventoryComponent>
{
    virtual void BindValues();
protected:
    InventoryComponentLoader();
    friend class ComponentLoaderFactory;
};


BOOST_CLASS_EXPORT_KEY2(InventoryComponent,"inventory_component");
#endif//INCLUDED_CORE_INVENTORY_COMPONENT_H
