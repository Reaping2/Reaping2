#include "core/armor_component.h"

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
