#ifndef INCLUDED_CORE_INVENTORY_COMPONENT_H
#define INCLUDED_CORE_INVENTORY_COMPONENT_H
#include "core/i_inventory_component.h"
#include "core/item_factory.h"
#include "core/property_loader.h"
#include "core/opt.h"
#include "core/weapon.h"
#include <boost/serialization/map.hpp>
#include "platform/export.h"
class InventoryComponent : public IInventoryComponent
{
public:
    virtual ItemMap_t const& GetItems()const;
    virtual ItemMap_t& GetItems();
    virtual Opt<Item> AddItem( int32_t Id );
    virtual Opt<Item> AddItem( std::unique_ptr<Item> item );
    virtual bool DropItem( int32_t Id );
    virtual Opt<Item> GetItem( int32_t Id );
    virtual Opt<Item> GetSelectedItem( ItemType::Type type );
    virtual Opt<Item> SetSelectedItem( ItemType::Type type, int32_t Id, bool force = false ) ;
    virtual void SetActorGUID( int32_t actorGUID );
    virtual Opt<Item> SwitchToNextItem( ItemType::Type itemType, bool forward = true );
    virtual void SetPickupItems( bool pickupItems );
    virtual bool IsPickupItems() const;
    virtual void SetDarkMatters( int32_t darkMatters );
    virtual int32_t GetDarkMatters() const;
    virtual void SetCollectDarkMatter( bool collectDarkMatter );
    virtual bool IsCollectDarkMatter() const;
    virtual ~InventoryComponent();
protected:
    InventoryComponent();
    friend class ComponentFactory;
private:
    ItemFactory& mItemFactory;
    ItemMap_t mItems;
    bool mPickupItems;
    int32_t mDarkMatters;
    bool mCollectDarkMatter;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void InventoryComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IInventoryComponent>( *this );
    ar& mItems;
    ar& mPickupItems;
    ar& mDarkMatters;
    ar& mCollectDarkMatter;
}

class InventoryComponentLoader: public ComponentLoader<InventoryComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( InventoryComponentLoader )
private:
    virtual void BindValues();
protected:
    InventoryComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( InventoryComponent, InventoryComponent, "inventory_component" );
#endif//INCLUDED_CORE_INVENTORY_COMPONENT_H
