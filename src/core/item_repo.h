#ifndef INCLUDED_CORE_ITEM_REPO_H
#define INCLUDED_CORE_ITEM_REPO_H

class Item;
class ItemRepo : public Factory<Item>, public Singleton<ItemRepo>
{
    friend class Singleton<ItemRepo>;
    ItemRepo();
};

#endif//INCLUDED_CORE_ITEM_REPO_H
