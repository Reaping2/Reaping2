#include "core/consumable.h"

Consumable::Consumable(bool active/*=false*/)
    : mActive(active)
    , mConsumed(false)
{
}

void Consumable::SetActive(bool active)
{
    mActive=active;
    if (!active)
    {
        SetConsumed(false);
    }
}

bool Consumable::IsActive()const
{
    return mActive;
}

void Consumable::SetConsumed(bool consumed)
{
    mConsumed=consumed;
}

bool Consumable::IsConsumed()const
{
    return mConsumed;
}

bool Consumable::GetValue() const
{
    return !mConsumed&&mActive;
}
