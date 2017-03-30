#ifndef INCLUDED_CORE_I_INVENTORY_COMPONENT_H
#define INCLUDED_CORE_I_INVENTORY_COMPONENT_H
#include "component.h"
#include "core/item.h"
#include "core/opt.h"
#include "weapon.h"
#include "normal_item.h"
#include "item_type.h"
#include "platform/export.h"
#include <map>
#include <memory>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
class IInventoryComponent : public Component
{
public:
    static constexpr int InvalidItem = -1;
protected:
    struct ItemGroup
    {
        typedef std::vector<std::shared_ptr<Item>> Items_t;
        Opt<Item> AddItem( std::unique_ptr<Item> item );
        Opt<Item> GetItem( int32_t Id );
        Opt<Item> GetSelectedItem();
        Opt<Item> SetSelectedItem( int32_t Id );
        bool DropItem( int32_t Id );
        Opt<Item> SwitchToNextItem( bool forward = true );
        void SetActorGUID( int32_t actorGUID );
    private:
        Items_t mItems;
        int32_t mSelectedIndex = InvalidItem;
        friend class ::boost::serialization::access;
        template<class Archive>
        void serialize( Archive& ar, const unsigned int version );
    };
public:
    DEFINE_COMPONENT_BASE( IInventoryComponent )
    typedef std::map<ItemType::Type, ItemGroup > ItemMap_t;
    virtual ItemMap_t const& GetItems()const = 0;
    virtual ItemMap_t& GetItems() = 0;
    virtual Opt<Item> AddItem( int32_t Id ) = 0;
    virtual Opt<Item> AddItem( std::unique_ptr<Item> item ) = 0;
    virtual bool DropItem( int32_t Id ) = 0;
    virtual Opt<Item> GetItem( int32_t Id ) = 0;
    virtual Opt<Item> GetSelectedItem( ItemType::Type type ) = 0;
    virtual Opt<Item> SetSelectedItem( ItemType::Type type, int32_t Id, bool force = false ) = 0;
    virtual Opt<Item> SwitchToNextItem( ItemType::Type itemType, bool forward = true ) = 0;
    virtual void SetPickupItems( bool pickupItems ) = 0;
    virtual bool IsPickupItems()const = 0;
    virtual void SetDarkMatters( int32_t darkMatters ) = 0;
    virtual int32_t GetDarkMatters()const = 0;
    virtual void SetCollectDarkMatter( bool collectDarkMatter ) = 0;
    virtual bool IsCollectDarkMatter()const = 0;
protected:
    friend class ComponentFactory;
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IInventoryComponent::ItemGroup::serialize( Archive& ar, const unsigned int version )
{
    ar& mItems;
    ar& mSelectedIndex;
}
template<class Archive>
void IInventoryComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IInventoryComponent, IInventoryComponent, "i_inventory_component" );
#endif//INCLUDED_CORE_I_INVENTORY_COMPONENT_H
