#include "core/armor_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

ArmorComponent::ArmorComponent()
    : mCurrentArmor(0)
{
}

void ArmorComponent::SetCurrentArmor(int32_t currentArmor)
{
    mCurrentArmor=currentArmor;
}

int32_t ArmorComponent::GetCurrentArmor()const
{
    return mCurrentArmor;
}



void ArmorComponentLoader::BindValues()
{
}

ArmorComponentLoader::ArmorComponentLoader()
{
}

BOOST_CLASS_EXPORT_IMPLEMENT(ArmorComponent);
