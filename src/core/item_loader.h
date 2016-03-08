#ifndef INCLUDED_CORE_ITEM_LOADER_H
#define INCLUDED_CORE_ITEM_LOADER_H
#include "property_loader.h"

template<typename T, typename BASE>
class PropertyLoader;
class Item;
template<typename ITEM>
class ItemLoader: public PropertyLoader<ITEM, Item>
{
};

#endif//INCLUDED_CORE_ITEM_LOADER_H