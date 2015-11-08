#include "normal_item.h"

NormalItem::NormalItem(int32_t Id)
    : Item(Id)
    , mUse(false)
    , mConsumed(false)
{
    mType = ItemType::Normal;
}

void NormalItem::SetUse(bool use)
{
    mUse=use;
}

bool NormalItem::IsUse() const
{
    return mUse;
}

void NormalItem::SetConsumed(bool consumed)
{
    mConsumed=consumed;
}

bool NormalItem::IsConsumed() const
{
    return mConsumed;
}
