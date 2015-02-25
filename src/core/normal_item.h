#ifndef INCLUDED_CORE_NORMAL_ITEM_H
#define INCLUDED_CORE_NORMAL_ITEM_H
#include "core/item.h"

class NormalItem : public Item
{
public:
    NormalItem( int32_t Id );
    virtual void SetUse(bool use);
    bool IsUse() const;

    void SetConsumed(bool consumed);
    bool IsConsumed() const;

    bool mUse;
    bool mConsumed;
};

#endif//INCLUDED_CORE_NORMAL_ITEM_H
