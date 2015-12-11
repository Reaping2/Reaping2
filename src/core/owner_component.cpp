#include "core/owner_component.h"

OwnerComponent::OwnerComponent()
    : mOwnerGUID(-1)
{
}

void OwnerComponent::SetOwnerGUID(int32_t ownerGUID)
{
    mOwnerGUID=ownerGUID;
}

int32_t OwnerComponent::GetOwnerGUID()const
{
    return mOwnerGUID;
}



void OwnerComponentLoader::BindValues()
{
}

OwnerComponentLoader::OwnerComponentLoader()
{
}
