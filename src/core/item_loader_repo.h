#ifndef INCLUDED_CORE_ITEM_LOADER_FACTORY_H
#define INCLUDED_CORE_ITEM_LOADER_FACTORY_H

#include "platform/singleton.h"
#include "platform/repository.h"
#include "platform/i_platform.h"

template<typename BASE>
class PropertyLoaderBase;
class Item;
class DefaultItemLoader;
class ItemLoaderRepo : public platform::Repository<PropertyLoaderBase<Item> >, public platform::Singleton<ItemLoaderRepo>
{
    friend class platform::Singleton<ItemLoaderRepo>;
    static DefaultItemLoader const mDefault;
    ItemLoaderRepo();
    void Init();
    bool LoadItemFromOneDesc( Json::Value& ItemDesc );
};

#endif//INCLUDED_CORE_ITEM_LOADER_FACTORY_H

//command:  "classgenerator" -g "factory" -c "item_loader_factory" -t "item_loader"
