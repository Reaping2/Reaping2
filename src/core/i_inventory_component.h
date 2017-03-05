#ifndef INCLUDED_CORE_I_INVENTORY_COMPONENT_H
#define INCLUDED_CORE_I_INVENTORY_COMPONENT_H
#include "component.h"
#include "core/item.h"
#include "core/opt.h"
#include "weapon.h"
#include "normal_item.h"
#include "item_type.h"
#include "platform/export.h"
#include <vector>

class IInventoryComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IInventoryComponent )
    typedef std::vector<Opt<Item> > ItemList_t;
    virtual ItemList_t const& GetItems()const = 0;
    virtual ItemList_t& GetItems() = 0;
    virtual void AddItem( int32_t Id ) = 0;
    virtual void AddItem( std::unique_ptr<Item> item ) = 0;
    virtual void DropItem( int32_t Id ) = 0;
    virtual Opt<Item> GetItem( int32_t Id ) = 0;
    virtual void DropItemType( ItemType::Type Type ) = 0;
    virtual Opt<Weapon> GetSelectedWeapon() = 0;
    virtual void SetSelectedWeapon( int32_t Id ) = 0;
    virtual Opt<NormalItem> GetSelectedNormalItem() = 0;
    virtual void SetSelectedNormalItem( int32_t Id ) = 0;
    virtual void SetPickupItems( bool pickupItems ) = 0;
    virtual bool IsPickupItems()const = 0;
    virtual void Update( double Seconds ) = 0;
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IInventoryComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IInventoryComponent, IInventoryComponent, "i_inventory_component" );
#endif//INCLUDED_CORE_I_INVENTORY_COMPONENT_H
