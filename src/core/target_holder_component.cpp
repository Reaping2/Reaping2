#include "core/target_holder_component.h"

TargetHolderComponent::TargetHolderComponent()
    : mTargetGUID(-1)
{
}

void TargetHolderComponent::SetTargetGUID(int32_t targetId)
{
    mTargetGUID=targetId;
}

int32_t TargetHolderComponent::GetTargetGUID()const
{
    return mTargetGUID;
}



void TargetHolderComponentLoader::BindValues()
{
}

TargetHolderComponentLoader::TargetHolderComponentLoader()
{
}
