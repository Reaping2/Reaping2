#include "core/cloak_component.h"

CloakComponent::CloakComponent()
    : mActive(false)
{
}

void CloakComponent::SetActive(bool active)
{
    mActive=active;
}

bool CloakComponent::IsActive()const
{
    return mActive;
}



void CloakComponentLoader::BindValues()
{
}

CloakComponentLoader::CloakComponentLoader()
{
}
