#include "core/listen_child_death_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

ListenChildDeathComponent::ListenChildDeathComponent()
    : mKillerOfChildGUID(-1)
{
}

void ListenChildDeathComponent::SetKillerOfChildGUID(int32_t killerOfChildId)
{
    mKillerOfChildGUID=killerOfChildId;
}

int32_t ListenChildDeathComponent::GetKillerOfChildGUID()const
{
    return mKillerOfChildGUID;
}



void ListenChildDeathComponentLoader::BindValues()
{
}

ListenChildDeathComponentLoader::ListenChildDeathComponentLoader()
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT(ListenChildDeathComponent, ListenChildDeathComponent);
