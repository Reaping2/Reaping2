#include "core/cloak_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

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

REAPING2_CLASS_EXPORT_IMPLEMENT(CloakComponent, CloakComponent);
