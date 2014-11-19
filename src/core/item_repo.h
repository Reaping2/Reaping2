#ifndef INCLUDED_CORE_ITEM_REPO_H
#define INCLUDED_CORE_ITEM_REPO_H

#include "platform/factory.h"
#include "platform/singleton.h"

class Item;
class ItemRepo : public platform::Factory<Item>, public platform::Singleton<ItemRepo>
{
    friend class platform::Singleton<ItemRepo>;
    ItemRepo();
};

#endif//INCLUDED_CORE_ITEM_REPO_H
