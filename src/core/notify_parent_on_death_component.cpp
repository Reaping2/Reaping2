#include "core/notify_parent_on_death_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

NotifyParentOnDeathComponent::NotifyParentOnDeathComponent()
    : mParentGUID(-1)
    , mKillerGUID(-1)
{
}

void NotifyParentOnDeathComponent::SetParentGUID(int32_t parentId)
{
    mParentGUID=parentId;
}

int32_t NotifyParentOnDeathComponent::GetParentGUID()const
{
    return mParentGUID;
}

void NotifyParentOnDeathComponent::SetKillerGUID(int32_t killerId)
{
    mKillerGUID=killerId;
}

int32_t NotifyParentOnDeathComponent::GetKillerGUID()const
{
    return mKillerGUID;
}



void NotifyParentOnDeathComponentLoader::BindValues()
{
}

NotifyParentOnDeathComponentLoader::NotifyParentOnDeathComponentLoader()
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT(NotifyParentOnDeathComponent, NotifyParentOnDeathComponent);
