#ifndef INCLUDED_CORE_ITEM_REPO_H
#define INCLUDED_CORE_ITEM_REPO_H

#include "platform/factory.h"
#include "platform/singleton.h"

class Item;
class ItemFactory : public platform::Factory<Item>, public platform::Singleton<ItemFactory>
{
    friend class platform::Singleton<ItemFactory>;
    ItemFactory();
};

#endif//INCLUDED_CORE_ITEM_REPO_H
