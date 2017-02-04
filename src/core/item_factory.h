#ifndef INCLUDED_CORE_ITEM_REPO_H
#define INCLUDED_CORE_ITEM_REPO_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "item_loader_repo.h"
#include "item_loader.h"

class Item;
class ItemFactory : public platform::Factory<Item>, public platform::Singleton<ItemFactory>
{
    friend class platform::Singleton<ItemFactory>;
    ItemFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<Item> CreateItem( int32_t Id );
};

template<typename Elem_T>
std::auto_ptr<Item> ItemFactory::CreateItem( int32_t Id )
{
    static ItemLoaderRepo& mItemLoaderRepo = ItemLoaderRepo::Get();
    return static_cast<ItemLoader<Item>& >( mItemLoaderRepo( Id ) )
           .FillProperties( std::auto_ptr<Item>( new Elem_T( Id ) ) );
}

#endif//INCLUDED_CORE_ITEM_REPO_H
